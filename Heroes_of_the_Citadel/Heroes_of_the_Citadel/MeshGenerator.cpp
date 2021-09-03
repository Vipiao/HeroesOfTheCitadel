


#include "MeshGenerator.h"

#include <unordered_map>
#include <algorithm>

void MeshGenerator::cube (std::vector<glm::vec3>* vertices, std::vector<int>* indices) {
   //return std::vector<glm::vec3> ();
   *vertices = {
      { 1.,  1.,  1.}, // RFU 0
      {-1.,  1.,  1.}, // LFU 1
      { 1., -1.,  1.}, // RBU 2
      {-1., -1.,  1.}, // LBU 3
      { 1.,  1., -1.}, // RFD 4
      {-1.,  1., -1.}, // LFD 5
      { 1., -1., -1.}, // RBD 6
      {-1., -1., -1.}, // LBD 7
   };
   *indices = {
      6, 3, 7,
      2, 3, 6,
      6, 4, 2,
      0, 2, 4,
      2, 1, 3,
      2, 0, 1,
      5, 7, 3,
      5, 3, 1,
      4, 5, 0,
      5, 1, 0,
      5, 4, 7,
      7, 4, 6,
   };
}

void MeshGenerator::cube (std::vector<double>* vertexData, std::vector<int>* indices) {
   *vertexData = {
      // Bottom.
      -1.0, -1.0, -1.0,  0.0,  0.0, -1.0,
       1.0,  1.0, -1.0,  0.0,  0.0, -1.0,
       1.0, -1.0, -1.0,  0.0,  0.0, -1.0,
       1.0,  1.0, -1.0,  0.0,  0.0, -1.0,
      -1.0, -1.0, -1.0,  0.0,  0.0, -1.0,
      -1.0,  1.0, -1.0,  0.0,  0.0, -1.0,
      // Top.
      -1.0, -1.0,  1.0,  0.0,  0.0, 1.0,
       1.0, -1.0,  1.0,  0.0,  0.0, 1.0,
       1.0,  1.0,  1.0,  0.0,  0.0, 1.0,
       1.0,  1.0,  1.0,  0.0,  0.0, 1.0,
      -1.0,  1.0,  1.0,  0.0,  0.0, 1.0,
      -1.0, -1.0,  1.0,  0.0,  0.0, 1.0,
      // Left.
      -1.0,  1.0,  1.0, -1.0,  0.0, 0.0,
      -1.0,  1.0, -1.0, -1.0,  0.0, 0.0,
      -1.0, -1.0, -1.0, -1.0,  0.0, 0.0,
      -1.0, -1.0, -1.0, -1.0,  0.0, 0.0,
      -1.0, -1.0,  1.0, -1.0,  0.0, 0.0,
      -1.0,  1.0,  1.0, -1.0,  0.0, 0.0,
      // Right
       1.0,  1.0,  1.0,  1.0,  0.0, 0.0,
       1.0, -1.0, -1.0,  1.0,  0.0, 0.0,
       1.0,  1.0, -1.0,  1.0,  0.0, 0.0,
       1.0, -1.0, -1.0,  1.0,  0.0, 0.0,
       1.0,  1.0,  1.0,  1.0,  0.0, 0.0,
       1.0, -1.0,  1.0,  1.0,  0.0, 0.0,
       // Back.
      -1.0, -1.0, -1.0,  0.0, -1.0,  0.0,
       1.0, -1.0, -1.0,  0.0, -1.0,  0.0,
       1.0, -1.0,  1.0,  0.0, -1.0,  0.0,
       1.0, -1.0,  1.0,  0.0, -1.0,  0.0,
      -1.0, -1.0,  1.0,  0.0, -1.0,  0.0,
      -1.0, -1.0, -1.0,  0.0, -1.0,  0.0,
      // Front.
      -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
       1.0,  1.0,  1.0,  0.0,  1.0,  0.0,
       1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
       1.0,  1.0,  1.0,  0.0,  1.0,  0.0,
      -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
      -1.0,  1.0,  1.0,  0.0,  1.0,  0.0,
   };
   *indices = {
       0, 1, 2, 3, 4, 5, // Bottom
       6, 7, 8, 9,10,11, // Top
      12,13,14,15,16,17, // Left
      18,19,20,21,22,23, // Right
      24,25,26,27,28,29, // Back
      30,31,32,33,34,35, // Front
   };
}

void MeshGenerator::sphere (std::vector<double>* vertexData, std::vector<int>* indices, int resolution, double radius) {

   // Create a base cube shape.
   std::vector<glm::dvec3> corners{
      glm::dvec3{-1,-1,-1}, // 0 Left back down.
      glm::dvec3{ 1,-1,-1}, // 1 Right back down.
      glm::dvec3{-1, 1,-1}, // 2 Left front down.
      glm::dvec3{ 1, 1,-1}, // 3 Right front down.
      glm::dvec3{-1,-1, 1}, // 4 Left back up.
      glm::dvec3{ 1,-1, 1}, // 5 Right back up.
      glm::dvec3{-1, 1, 1}, // 6 Left front up.
      glm::dvec3{ 1, 1, 1}, // 7 Right front up.
   };
   std::vector<glm::ivec3> triangles{
      glm::ivec3{1, 3, 5}, glm::ivec3{3, 7, 5}, // Right.
      glm::ivec3{0, 4, 2}, glm::ivec3{4, 6, 2}, // Left.
      glm::ivec3{2, 6, 3}, glm::ivec3{6, 7, 3}, // Front.
      glm::ivec3{0, 1, 4}, glm::ivec3{1, 5, 4}, // Back.
      glm::ivec3{4, 5, 6}, glm::ivec3{5, 7, 6}, // Top.
      glm::ivec3{0, 2, 1}, glm::ivec3{2, 3, 1}, // Bottom.
   };

   // Increase resolution.
   std::unordered_map<unsigned int, int> umap;
   for (size_t ii = 0; ii < resolution; ii++) {
      umap = {}; // Reset umap.
      size_t numTriangles = triangles.size ();
      unsigned int key;
      for (size_t jj = 0; jj < numTriangles; jj++) {
         const glm::ivec3 t = triangles[jj];

         // Get in between corners.
         int c0, c1, c2;
         // C0.
         key = std::min (t.x, t.y) + (std::max (t.x, t.y) << 16);
         if (umap.find (key) == umap.end ()) {
            c0 = umap[key] = (int)corners.size ();
            corners.push_back (0.5 * (corners[t.x] + corners[t.y]));
         } else {
            c0 = umap[key];
         }
         // C1.
         key = std::min (t.y, t.z) + (std::max (t.y, t.z) << 16);
         if (umap.find (key) == umap.end ()) {
            c1 = umap[key] = (int)corners.size ();
            corners.push_back (0.5 * (corners[t.y] + corners[t.z]));
         } else {
            c1 = umap[key];
         }
         // C2.
         key = std::min (t.z, t.x) + (std::max (t.z, t.x) << 16);
         if (umap.find (key) == umap.end ()) {
            c2 = umap[key] = (int)corners.size ();
            corners.push_back (0.5 * (corners[t.z] + corners[t.x]));
         } else {
            c2 = umap[key];
         }

         // Create new triangles.
         triangles.push_back (glm::ivec3{ t.x, c0, c2 });
         triangles.push_back (glm::ivec3{ t.y, c1, c0 });
         triangles.push_back (glm::ivec3{ t.z, c2, c1 });
         triangles[jj] = glm::ivec3{c0, c1, c2};
      }
   }

   // Map to sphere.
   for (size_t ii = 0; ii < corners.size (); ii++) {
      corners[ii] = glm::normalize (corners[ii]) * radius;
   }

   // Format data.
   *vertexData = {};
   for (size_t ii = 0; ii < corners.size(); ii++) {
      // Position.
      vertexData->push_back (corners[ii].x);
      vertexData->push_back (corners[ii].y);
      vertexData->push_back (corners[ii].z);
      // Normal.
      vertexData->push_back (corners[ii].x);
      vertexData->push_back (corners[ii].y);
      vertexData->push_back (corners[ii].z);
      // Texture.
      vertexData->push_back (0.0);
      vertexData->push_back (0.0);
   }
   *indices = {};
   for (size_t ii = 0; ii < triangles.size (); ii++) {
      indices->push_back (triangles[ii].x);
      indices->push_back (triangles[ii].y);
      indices->push_back (triangles[ii].z);
   }
}

void MeshGenerator::plane (std::vector<double>* vertexData, std::vector<int>* indices) {
   *vertexData = {
      -1.0, -1.0,  0.0,  0.0,  0.0, 1.0,
       1.0, -1.0,  0.0,  0.0,  0.0, 1.0,
       1.0,  1.0,  0.0,  0.0,  0.0, 1.0,
       1.0,  1.0,  0.0,  0.0,  0.0, 1.0,
      -1.0,  1.0,  0.0,  0.0,  0.0, 1.0,
      -1.0, -1.0,  0.0,  0.0,  0.0, 1.0,
   };
   *indices = {
       0, 1, 2, 3, 4, 5,
   };
}

void MeshGenerator::crosshair (std::vector<double>* vertexData, std::vector<int>* indices) {
   double outer = 1.3;
   double inner = 1.0;
   double margin = 0.7;
   *vertexData = {
      // Right.
       inner, -inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       outer, -inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       outer,  inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       inner,  inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
      // Left.
       -outer, -inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       -inner, -inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       -inner,  inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       -outer,  inner * margin,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
      // Top.
       -inner * margin,  inner,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
        inner * margin,  inner,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
        inner * margin,  outer,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       -inner * margin,  outer,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
      // Bottom.
       -inner * margin, -outer,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
        inner * margin, -outer,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
        inner * margin, -inner,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
       -inner * margin, -inner,  0.0,  0.0,  0.0, 1.0,// 0.0, 0.0,
   };
   *indices = {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9, 10,  8, 10, 11,
       12, 13, 14, 12, 14, 15,
   };
}
