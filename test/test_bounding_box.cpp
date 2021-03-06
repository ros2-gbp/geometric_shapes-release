// Copyright 2019 Open Robotics
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the Open Robotics nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/** \Author Martin Pecka */

#include <geometric_shapes/bodies.h>
#include <geometric_shapes/body_operations.h>
#include <random_numbers/random_numbers.h>
#include <gtest/gtest.h>

// The magic numbers in this test were verified visually using Blender

TEST(SphereBoundingBox, Sphere1)
{
  shapes::Sphere shape(1.0);
  bodies::Sphere body(&shape);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-1.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().z(), 1e-4);
}

TEST(SphereBoundingBox, Sphere2)
{
  shapes::Sphere shape(2.0);
  bodies::Sphere body(&shape);
  Eigen::Isometry3d pose;
  pose.setIdentity();
  pose.translation() = Eigen::Vector3d(1, 2, 3);
  body.setPose(pose);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-1.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(0.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(3.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(4.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(5.0, bbox.max().z(), 1e-4);

  pose *= Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d(1, 1, 1).normalized());
  body.setPose(pose);
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-1.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(0.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(3.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(4.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(5.0, bbox.max().z(), 1e-4);

  // verify the bounding box is rotation-invariant

  random_numbers::RandomNumberGenerator gen;
  double quatData[4];
  Eigen::Quaterniond quat;

  for (size_t i = 0; i < 10; ++i)
  {
    gen.quaternion(quatData);
    quat.x() = quatData[0];
    quat.y() = quatData[1];
    quat.z() = quatData[2];
    quat.w() = quatData[3];
    pose.linear() = quat.toRotationMatrix();
    body.setPose(pose);
    bodies::AABB bbox2;
    body.computeBoundingBox(bbox2);

    EXPECT_NEAR(bbox2.min().x(), bbox.min().x(), 1e-4);
    EXPECT_NEAR(bbox2.min().y(), bbox.min().y(), 1e-4);
    EXPECT_NEAR(bbox2.min().z(), bbox.min().z(), 1e-4);
    EXPECT_NEAR(bbox2.max().x(), bbox.max().x(), 1e-4);
    EXPECT_NEAR(bbox2.max().y(), bbox.max().y(), 1e-4);
    EXPECT_NEAR(bbox2.max().z(), bbox.max().z(), 1e-4);
  }
}

TEST(BoxBoundingBox, Box1)
{
  shapes::Box shape(1.0, 2.0, 3.0);
  bodies::Box body(&shape);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-0.5, bbox.min().x(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(-1.5, bbox.min().z(), 1e-4);
  EXPECT_NEAR(0.5, bbox.max().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(1.5, bbox.max().z(), 1e-4);
}

TEST(BoxBoundingBox, Box2)
{
  shapes::Box shape(1.0, 2.0, 3.0);
  bodies::Box body(&shape);
  Eigen::Isometry3d pose;
  pose.setIdentity();
  pose.translation() = Eigen::Vector3d(1, 2, 3);
  body.setPose(pose);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(0.5, bbox.min().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.5, bbox.min().z(), 1e-4);
  EXPECT_NEAR(1.5, bbox.max().x(), 1e-4);
  EXPECT_NEAR(3.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(4.5, bbox.max().z(), 1e-4);

  pose *= Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d(1, 1, 1).normalized());
  body.setPose(pose);
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-0.7767, bbox.min().x(), 1e-4);
  EXPECT_NEAR(0.8452, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.4673, bbox.min().z(), 1e-4);
  EXPECT_NEAR(2.7767, bbox.max().x(), 1e-4);
  EXPECT_NEAR(3.1547, bbox.max().y(), 1e-4);
  EXPECT_NEAR(4.5326, bbox.max().z(), 1e-4);
}

TEST(CylinderBoundingBox, Cylinder1)
{
  shapes::Cylinder shape(1.0, 2.0);
  bodies::Cylinder body(&shape);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-1.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().z(), 1e-4);
}

TEST(CylinderBoundingBox, Cylinder2)
{
  shapes::Cylinder shape(1.0, 2.0);
  bodies::Cylinder body(&shape);
  Eigen::Isometry3d pose;
  pose.setIdentity();
  pose.translation() = Eigen::Vector3d(1, 2, 3);
  body.setPose(pose);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(0.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(2.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(2.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(3.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(4.0, bbox.max().z(), 1e-4);

  pose *= Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d(1, 1, 1).normalized());
  body.setPose(pose);
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-0.3238, bbox.min().x(), 1e-4);
  EXPECT_NEAR(0.7862, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.7239, bbox.min().z(), 1e-4);
  EXPECT_NEAR(2.3238, bbox.max().x(), 1e-4);
  EXPECT_NEAR(3.2138, bbox.max().y(), 1e-4);
  EXPECT_NEAR(4.2761, bbox.max().z(), 1e-4);

  // verify the bounding box is yaw-invariant

  random_numbers::RandomNumberGenerator gen(0);
  const auto rollPitch =
      Eigen::AngleAxisd(1.0, Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(1.0, Eigen::Vector3d::UnitY());

  pose.linear() = rollPitch.toRotationMatrix();
  body.setPose(pose);
  body.computeBoundingBox(bbox);

  bodies::AABB bbox2;
  for (size_t i = 0; i < 10; ++i)
  {
    const auto angle = gen.uniformReal(-M_PI, M_PI);
    const auto yaw = Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitZ());
    pose.linear() = (rollPitch * yaw).toRotationMatrix();
    body.setPose(pose);
    body.computeBoundingBox(bbox2);

    EXPECT_NEAR(bbox2.min().x(), bbox.min().x(), 1e-4);
    EXPECT_NEAR(bbox2.min().y(), bbox.min().y(), 1e-4);
    EXPECT_NEAR(bbox2.min().z(), bbox.min().z(), 1e-4);
    EXPECT_NEAR(bbox2.max().x(), bbox.max().x(), 1e-4);
    EXPECT_NEAR(bbox2.max().y(), bbox.max().y(), 1e-4);
    EXPECT_NEAR(bbox2.max().z(), bbox.max().z(), 1e-4);
  }
}

shapes::Mesh* createBoxMesh(const Eigen::Vector3d& min, const Eigen::Vector3d& max)
{
  shapes::Mesh* m = new shapes::Mesh(8, 12);

  m->vertices[3 * 0 + 0] = min.x();
  m->vertices[3 * 0 + 1] = min.y();
  m->vertices[3 * 0 + 2] = min.z();

  m->vertices[3 * 1 + 0] = max.x();
  m->vertices[3 * 1 + 1] = min.y();
  m->vertices[3 * 1 + 2] = min.z();

  m->vertices[3 * 2 + 0] = min.x();
  m->vertices[3 * 2 + 1] = max.y();
  m->vertices[3 * 2 + 2] = min.z();

  m->vertices[3 * 3 + 0] = max.x();
  m->vertices[3 * 3 + 1] = max.y();
  m->vertices[3 * 3 + 2] = min.z();

  m->vertices[3 * 4 + 0] = min.x();
  m->vertices[3 * 4 + 1] = min.y();
  m->vertices[3 * 4 + 2] = max.z();

  m->vertices[3 * 5 + 0] = max.x();
  m->vertices[3 * 5 + 1] = min.y();
  m->vertices[3 * 5 + 2] = max.z();

  m->vertices[3 * 6 + 0] = min.x();
  m->vertices[3 * 6 + 1] = max.y();
  m->vertices[3 * 6 + 2] = max.z();

  m->vertices[3 * 7 + 0] = max.x();
  m->vertices[3 * 7 + 1] = max.y();
  m->vertices[3 * 7 + 2] = max.z();

  m->triangles[3 * 0 + 0] = 0;
  m->triangles[3 * 0 + 1] = 1;
  m->triangles[3 * 0 + 2] = 2;

  m->triangles[3 * 1 + 0] = 1;
  m->triangles[3 * 1 + 1] = 3;
  m->triangles[3 * 1 + 2] = 2;

  m->triangles[3 * 2 + 0] = 5;
  m->triangles[3 * 2 + 1] = 4;
  m->triangles[3 * 2 + 2] = 6;

  m->triangles[3 * 3 + 0] = 5;
  m->triangles[3 * 3 + 1] = 6;
  m->triangles[3 * 3 + 2] = 7;

  m->triangles[3 * 4 + 0] = 1;
  m->triangles[3 * 4 + 1] = 5;
  m->triangles[3 * 4 + 2] = 3;

  m->triangles[3 * 5 + 0] = 5;
  m->triangles[3 * 5 + 1] = 7;
  m->triangles[3 * 5 + 2] = 3;

  m->triangles[3 * 6 + 0] = 4;
  m->triangles[3 * 6 + 1] = 0;
  m->triangles[3 * 6 + 2] = 2;

  m->triangles[3 * 7 + 0] = 4;
  m->triangles[3 * 7 + 1] = 2;
  m->triangles[3 * 7 + 2] = 6;

  m->triangles[3 * 8 + 0] = 2;
  m->triangles[3 * 8 + 1] = 3;
  m->triangles[3 * 8 + 2] = 6;

  m->triangles[3 * 9 + 0] = 3;
  m->triangles[3 * 9 + 1] = 7;
  m->triangles[3 * 9 + 2] = 6;

  m->triangles[3 * 10 + 0] = 1;
  m->triangles[3 * 10 + 1] = 0;
  m->triangles[3 * 10 + 2] = 4;

  m->triangles[3 * 11 + 0] = 1;
  m->triangles[3 * 11 + 1] = 4;
  m->triangles[3 * 11 + 2] = 5;

  return m;
}

TEST(MeshBoundingBox, Mesh1)
{
  shapes::Mesh* m = createBoxMesh({ -1, -1, -1 }, { 1, 1, 1 });

  bodies::ConvexMesh body(m);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-1.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().z(), 1e-4);
  delete m;
}

TEST(MeshBoundingBox, Mesh2)
{
  shapes::Mesh* m = createBoxMesh({ -0.5, -1.0, -1.5 }, { 0.5, 1.0, 1.5 });

  bodies::ConvexMesh body(m);
  Eigen::Isometry3d pose;
  pose.setIdentity();
  pose.translation() = Eigen::Vector3d(1, 2, 3);
  body.setPose(pose);
  bodies::AABB bbox;
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(0.5, bbox.min().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.5, bbox.min().z(), 1e-4);
  EXPECT_NEAR(1.5, bbox.max().x(), 1e-4);
  EXPECT_NEAR(3.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(4.5, bbox.max().z(), 1e-4);

  pose *= Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d(1, 1, 1).normalized());
  body.setPose(pose);
  body.computeBoundingBox(bbox);

  EXPECT_NEAR(-0.7767, bbox.min().x(), 1e-4);
  EXPECT_NEAR(0.8452, bbox.min().y(), 1e-4);
  EXPECT_NEAR(1.4673, bbox.min().z(), 1e-4);
  EXPECT_NEAR(2.7767, bbox.max().x(), 1e-4);
  EXPECT_NEAR(3.1547, bbox.max().y(), 1e-4);
  EXPECT_NEAR(4.5326, bbox.max().z(), 1e-4);

  delete m;
}

TEST(MergeBoundingBoxes, Merge1)
{
  std::vector<bodies::AABB> boxes;
  boxes.emplace_back(Eigen::Vector3d(-1, -1, -1), Eigen::Vector3d(0, 0, 0));
  boxes.emplace_back(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(1, 1, 1));

  bodies::AABB bbox;
  bodies::mergeBoundingBoxes(boxes, bbox);

  EXPECT_NEAR(-1.0, bbox.min().x(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().y(), 1e-4);
  EXPECT_NEAR(-1.0, bbox.min().z(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().x(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().y(), 1e-4);
  EXPECT_NEAR(1.0, bbox.max().z(), 1e-4);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
