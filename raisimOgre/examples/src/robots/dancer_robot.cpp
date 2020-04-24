#include <raisim/OgreVis.hpp>
#include "raisimBasicImguiPanel.hpp"
#include "helper.hpp"

void setupCallback() {
  auto vis = raisim::OgreVis::get();

  /// light
  vis->getLight()->setDiffuseColour(1, 1, 1);
  vis->getLight()->setCastShadows(true);
  vis->getLightNode()->setPosition(3, 3, 3);

  /// 建立地图
  vis->addResourceDirectory(vis->getResourceDir() + "/material/checkerboard");
  vis->loadMaterialFile("checkerboard.material");

  /// shadow setting
  vis->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
  vis->getSceneManager()->setShadowTextureSettings(2048, 3);

  /// scale related settings!! Please adapt it depending on your map size
  // beyond this distance, shadow disappears在这段距离之外，阴影消失了
  vis->getSceneManager()->setShadowFarDistance(10);
  // size of contact points and contact forces接触点尺寸和接触力
  vis->setContactVisObjectSize(0.03, 0.2);
  // speed of camera motion in freelook modefreelook模式下的相机运动速度
  vis->getCameraMan()->setTopSpeed(1);

  /// 建立天空球
  Ogre::Quaternion quat;
  quat.FromAngleAxis(Ogre::Radian(M_PI_2), {1., 0, 0});
  vis->getSceneManager()->setSkyBox(true,
                                    "Examples/StormySkyBox",
                                    500,
                                    true,
                                    quat);
}

int main(int argc, char **argv) {
  /// create raisim world
  raisim::World world;
  world.setTimeStep(0.002);

  /// just a shortcut
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

  /// 创建机器人模型
  auto robot = world.addArticulatedSystem(raisim::loadResource("dancer_model/dancer_urdf_model.urdf"));
  robot->setName("dancer_robot");

  /// 加载地图图片
  auto ground = world.addGround();
  ground->setName("checkerboard");
  vis->createGraphicalObject(ground, 10, "floor", "robot_world");
  auto robot_visual = vis->createGraphicalObject(robot, "dancer_robot");

  /// ？
  Eigen::VectorXd gc(36);
  gc.setZero();
  gc.segment<7>(0) << 0, 0, 1, 1, 0, 0, 0;
  robot->setGeneralizedCoordinate(gc);

  /// 选择初始视角
  vis->select(robot_visual->at(0));
  vis->getCameraMan()->setYawPitchDist(Ogre::Radian(0), Ogre::Radian(-1.f), 10);

  auto dancerFromWorld = world.getObject("dancer_robot");
  std::cout<<"dancer_robot name is "<<dancerFromWorld->getName()<<std::endl;

  /// run the app
  vis->run();

  /// terminate
  vis->closeApp();

  return 0;
}
