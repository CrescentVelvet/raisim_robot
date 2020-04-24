#include "raisim/World.hpp"
#include "raisim/RaisimServer.hpp"
#include "helper.hpp"

int main() {

  /// create raisim world
  raisim::World world;
  world.setTimeStep(0.003);
  world.setERP(world.getTimeStep(), world.getTimeStep());


  /// create objects
  auto ground = world.addGround();
  auto ball = world.addSphere(1,1);

  std::vector<raisim::ArticulatedSystem*> atlas;

  const size_t N = 1;

  for(size_t i=0; i<N; i++) {
    for(size_t j=0; j<N; j++) {
      atlas.push_back(world.addArticulatedSystem(raisim::loadResource("dancer_model/dancer_urdf_model.URDF")));
      atlas.back()->setGeneralizedCoordinate({double(2 * i), double(j), 1.0, 1.0, 0.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0});
      atlas.back()->setGeneralizedForce(Eigen::VectorXd::Zero(atlas.back()->getDOF()));
      atlas.back()->setName("dancer_model" + std::to_string(j + i * N));
    }
  }

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0.0, 0.2);
  std::srand(std::time(nullptr));
  atlas.back()->printOutBodyNamesInOrder();

  /// launch raisim servear
  raisim::RaisimServer server(&world);
  server.launchServer();

  while(1) {
    raisim::MSLEEP(2);
    server.integrateWorldThreadSafe();
  }

  server.killServer();
}
