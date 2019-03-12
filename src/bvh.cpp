#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c, alpha);
  } else {
    draw(node->l, c, alpha);
    draw(node->r, c, alpha);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c, alpha);
  } else {
    drawOutline(node->l, c, alpha);
    drawOutline(node->r, c, alpha);
  }
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
  
  // TODO (Part 2.1):
  // Construct a BVH from the given vector of primitives and maximum leaf
  // size configuration. The starter code build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.
  BBox centroid_box, bbox;

  for (Primitive *p : prims) {
    BBox bb = p->get_bbox();
    bbox.expand(bb);
    Vector3D c = bb.centroid();
    centroid_box.expand(c);
  }


  BVHNode *node = new BVHNode(bbox);

  if (prims.size() <= max_leaf_size) {
    node -> prims = new vector<Primitive*>(prims);
    return node;
  } else {//Split the node
    //0 x axis, 1 y axis, 2 z axis;
    Vector3D extent = node->bb.extent;
    Vector3D min = node->bb.min;

    int split_axis = extent[0] > extent[1] ? 0 : 1;
    split_axis = extent[split_axis] > extent[2] ? split_axis : 2;

    //calculating the midpoint
    double midpoint = min[split_axis] + extent[split_axis] / 2;

    //split the primitives
    vector<Primitive*> *left = new vector<Primitive*>();
    vector<Primitive*> *right = new vector<Primitive*>();

    for (Primitive *p: prims) {
      if (p->get_bbox().centroid()[split_axis] < midpoint) {
        left->push_back(p);
      } else {
        right->push_back(p);
      }
    }

    while (left->empty() || right->empty()) {//Use average centroid if one of the left or right is empty.
      left->clear();
      right->clear();

      double avg_centroid = 0.0;

      for (Primitive *p: prims) {
        avg_centroid += p->get_bbox().centroid()[split_axis];
      }

      avg_centroid = avg_centroid / (double) prims.size();

      for (Primitive *p: prims) {
        if (p->get_bbox().centroid()[split_axis] < avg_centroid) {
          left->push_back(p);
        } else {
          right->push_back(p);
        }
      }

      if (left->empty() && !right->empty()) {
        node->prims = right;
        return node;
      } else if (right->empty() && !left->empty()) {
        node->prims = left;
        return node;
      }
    }

    node->l = construct_bvh(*left, max_leaf_size);
    node->r = construct_bvh(*right, max_leaf_size);
    return node;
  }
}


bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {
  // TODO (Part 2.3):
  // Fill in the intersect function.
  // Take note that this function has a short-circuit that the
  // Intersection version cannot, since it returns as soon as it finds
  // a hit, it doesn't actually have to find the closest hit.
  double t0 = -INFINITY;
  double t1 = INFINITY;

  if (!node->bb.intersect(ray, t0 ,t1)) {
    return false;
  }

  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims)) {
      total_isects++;
      if (node->bb.intersect(ray, t0, t1)) {
        if (p->intersect(ray)) {
          return true;
        }
      }
    }
    return false;
  } else {
    return intersect(ray, node->l) || intersect(ray, node->r);
  }

}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {

  // TODO (Part 2.3):
  // Fill in the intersect function.

  bool hit = false;

  double t0 = -INFINITY;
  double t1 = INFINITY;

  if (!node->bb.intersect(ray, t0 ,t1)) {
    return hit;
  }

  if (t0 > ray.max_t || t1 < ray.min_t) {
    return false;
  }

  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims)) {
      total_isects++;
      if (p->intersect(ray, i)) {
        hit = true;
      }
    }
  } else {
    bool hit1 = intersect(ray, i, node->l);
    bool hit2 = intersect(ray, i, node->r);
    hit = hit1 || hit2;
  }
  return hit;
}

}  // namespace StaticScene
}  // namespace CGL
