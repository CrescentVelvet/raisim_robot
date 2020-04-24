#include <raisim/OgreVis.hpp>
#include "raisimBasicImguiPanel.hpp"
#include "helper.hpp"

void setupCallback() {
  auto vis = raisim::OgreVis::get();

  /// light
  vis->getLight()->setDiffuseColour(1, 1, 1);
  vis->getLight()->setCastShadows(true);
  vis->getLightNode()->setPosition(3, 3, 3);

  /// load textures
  vis->addResourceDirectory(vis->getResourceDir() + "/material/checkerboard");
  vis->loadMaterialFile("checkerboard.material");

  vis->addResourceDirectory(vis->getResourceDir() + "/material/skybox/violentdays");
  vis->loadMaterialFile("violentdays.material");

  /// shdow setting
  vis->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
  vis->getSceneManager()->setShadowTextureSettings(2048, 3);

  /// scale related settings!! Please adapt it depending on your map size
  // beyond this distance, shadow disappears
  vis->getSceneManager()->setShadowFarDistance(10);
  // size of contact points and contact forces
  vis->setContactVisObjectSize(0.03, 0.2);
  // speed of camera motion in freelook mode
  vis->getCameraMan()->setTopSpeed(5);

  /// skybox
  Ogre::Quaternion quat;
  quat.FromAngleAxis(Ogre::Radian(M_PI_2), {1., 0, 0});
  vis->getSceneManager()->setSkyBox(true,
                                    "skybox/violentdays",
                                    500,
                                    true,
                                    quat,
                                    Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
}

int main(int argc, char **argv) {
  /// create raisim world
  raisim::World world;
  world.setTimeStep(0.002);

  auto vis = raisim::OgreVis::get();

  /// these method must be called before initApp
  vis->setWorld(&world);
  vis->setWindowSize(1800, 1200);
  vis->setImguiSetupCallback(imguiSetupCallback);
  vis->setImguiRenderCallback(imguiRenderCallBack);
  vis->setSetUpCallback(setupCallback);
  vis->setAntiAliasing(8);

  /// starts visualizer thread
  vis->initApp();

  /// create raisim objects
  auto ground = world.addGround();

  auto robot = world.addArticulatedSystem(raisim::loadResource("dancer_model/dancer_urdf_model.urdf"));
  robot->setName("atlas");
  Eigen::VectorXd gc(36);
  gc.setZero();
  gc.segment<7>(0) << 0, 0, 1, 1, 0, 0, 0;

  robot->setGeneralizedCoordinate(gc);

  /// create visualizer objects
  vis->createGraphicalObject(ground, 10, "floor", "checkerboard_green_transparent");
  auto robot_visual = vis->createGraphicalObject(robot, "atlas");

  vis->select(robot_visual->at(0));
  vis->getCameraMan()->setYawPitchDist(Ogre::Radian(0), Ogre::Radian(-1.f), 10);

  auto atlasFromWorld = world.getObject("atlas");
  std::cout<<"atlas name "<<atlasFromWorld->getName()<<std::endl;

  /// run the app
  vis->run();

  /// terminate
  vis->closeApp();

  return 0;
}
