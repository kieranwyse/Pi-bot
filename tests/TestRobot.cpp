/**
* @Author: Kieran Wyse
* @Date:   28-10-2016
* @Email:  kieranwyse@gmail.com
* @Project: Pi-Bot
* @Last modified by:   Kieran Wyse
* @Last modified time: 19-11-2016
* @License: GPL v3
*     This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Robot.hpp"
#include "sstream"
#include <time.h>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch/catch.hpp"



using namespace std;
TEST_CASE( "Robot object", "test methods" ) {
  //Test the constructor
  Robot *test = new Robot();


  int sensorPinLeft = 12;
  int sensorPinRight = 11;

  int forwardLeftpin = 9;
  int forwardRightpin = 10;

  int backwardLeftpin = 6;
  int backwardRightpin = 5;

  //Have to tell the system what pins are attached to the left and right wheel sensors
  sensorPin[forwardLeftpin] = sensorPinLeft;
  sensorPin[backwardLeftpin] = sensorPinLeft;
  //Have to tell the system how many ticks are on each of the wheel sensors
  ticks[sensorPinLeft] = 20;

  sensorPin[forwardRightpin] = sensorPinRight;
  sensorPin[backwardRightpin] = sensorPinRight;
  ticks[sensorPinRight] = 20;
  //test get

  SECTION("test get methods"){
    REQUIRE(test->getX() == 0);
    REQUIRE(test->getY() == 0);
    REQUIRE(test->getAngle() == 0);
    REQUIRE(test->getTargetX() == 0);
    REQUIRE(test->getTargetY() == 0);
    REQUIRE(test->getTargetAngle() == 0);
    delete test;

  }

  SECTION("drive method"){
    test->start();
    test->drive(50);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    test->stop();
    cout<<*test;
    REQUIRE(test->getX() != 0);
    delete test;
  }
  SECTION("rotate robot"){
    std::cout <<"rotating to pi/2" << std::endl;
    test->start();
    test->rotateTo(M_PI/2);
    REQUIRE(test->getTargetAngle() == M_PI/2 );
    for(int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::cout << "Angle is at:" << test->getAngle() << std::endl;
      Json::Value bot = test->getJSON();
      std::cout <<"left wheel" << bot["left-wheel"]["wheel-encoder"]["forward"]<< std::endl;;
      std::cout <<"right wheel" << bot["right-wheel"]["wheel-encoder"]["forward"] << std::endl;
    }
    test->stop();
    REQUIRE(test->getAngle() == Approx( M_PI/2 ).epsilon( 0.1 ));
    test->start();
    std::cout <<"rotating to 0" << std::endl;
    test->rotateTo(0);
    for(int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::cout << "Angle is at:" << test->getAngle() << std::endl;
    }

    test->stop();
    REQUIRE(test->getAngle() == Approx( 0 ).epsilon( 0.1 ));
    test->start();
    std::cout <<"rotating to -pi/2" << std::endl;
    test->rotateTo( -M_PI/2);
    for(int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::cout << "Angle is at:" << test->getAngle() << std::endl;
    }
    test->stop();
    REQUIRE(test->getAngle() == Approx( -M_PI/2 ).epsilon( 0.1 ));
    test->start();
    std::cout <<"rotating to pi" << std::endl;
    test->rotateTo( M_PI);
    for(int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::cout << "Angle is at:" << test->getAngle() << std::endl;
    }
    test->stop();

    double a = test->getAngle();
    if( a < 0) {
      CHECK( a == Approx( -M_PI ).epsilon( 0.1 ));
    } else {
      CHECK( a == Approx( M_PI ).epsilon( 0.1 ));
    }

    std::cout <<"rotating to 0 or 2pi" << std::endl;
    test->start();
    test->rotateTo( 2*M_PI);
    for(int i = 0; i < 10; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::cout << "Angle is at:" << test->getAngle() << std::endl;

    }
    test->stop();

    REQUIRE(test->getAngle() == Approx(0 ).epsilon( 0.1 ));
    delete test;

  }


}
TEST_CASE( "Robot move", "test move method" ) {

  Robot *test = new Robot();

  int sensorPinLeft = 12;
  int sensorPinRight = 11;

  int forwardLeftpin = 9;
  int forwardRightpin = 10;

  int backwardLeftpin = 6;
  int backwardRightpin = 5;

  //Have to tell the system what pins are attached to the left and right wheel sensors
  sensorPin[forwardLeftpin] = sensorPinLeft;
  sensorPin[backwardLeftpin] = sensorPinLeft;
  //Have to tell the system how many ticks are on each of the wheel sensors
  ticks[sensorPinLeft] = 20;

  sensorPin[forwardRightpin] = sensorPinRight;
  sensorPin[backwardRightpin] = sensorPinRight;
  ticks[sensorPinRight] = 20;
  //test get

  SECTION("Test forward"){
    test->start();
    test->forwardTo(10);
    while( test->getDistance() < 10) {
      std::cout <<"robot at distance:" <<test->getDistance()<< std::endl;
      Json::Value value = test->getJSON();
      std::cout <<"robot at x:" <<test->getX()<<" y at:"<<test->getY() <<" angle:"<<value.get("angle","none") <<" target angle:" << value.get("target-angle","none")<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getDistance() ==Approx(10 ).epsilon( 0.1 ));
    REQUIRE(test->getX() ==Approx(10 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(0 ).epsilon( 0.1 ));


  }
  delete test;
}


TEST_CASE( "Robot goto", "test move method" ) {

  Robot *test = new Robot();

  int sensorPinLeft = 12;
  int sensorPinRight = 11;

  int forwardLeftpin = 9;
  int forwardRightpin = 10;

  int backwardLeftpin = 6;
  int backwardRightpin = 5;

  //Have to tell the system what pins are attached to the left and right wheel sensors
  sensorPin[forwardLeftpin] = sensorPinLeft;
  sensorPin[backwardLeftpin] = sensorPinLeft;
  //Have to tell the system how many ticks are on each of the wheel sensors
  ticks[sensorPinLeft] = 20;

  sensorPin[forwardRightpin] = sensorPinRight;
  sensorPin[backwardRightpin] = sensorPinRight;
  ticks[sensorPinRight] = 20;
  //test get

  SECTION("Test goto"){
    test->start();
    test->goTo(1,1);

    REQUIRE(test->getTargetX() == 1);
    REQUIRE(test->getTargetY() == 1);

    for(int i = 0; i < 40 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY() <<value["state"]<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    test->stop();

    REQUIRE(test->getX() ==Approx(1 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(1 ).epsilon( 0.1 ));

    test->start();
    test->goTo(4,7);
    REQUIRE(test->getTargetX() == 4);
    REQUIRE(test->getTargetY() == 7);
    test->start();
    for(int i = 0; i < 40 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY() <<value["state"]<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getX() ==Approx(4 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(7 ).epsilon( 0.1 ));

    test->goTo(-4,-3);
    REQUIRE(test->getTargetX() == -4);
    REQUIRE(test->getTargetY() == -3);

    for(int i = 0; i < 40 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY() <<value["state"]<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getX() ==Approx(-4 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(-3 ).epsilon( 0.1 ));


    delete test;



  }

  SECTION("Test targers"){
    test->goTo(1,1);
    test->goTo(2,-7);
    test->goTo(-1,3);
    REQUIRE(test->getTargetX() == 1);
    REQUIRE(test->getTargetY() == 1);
    test->start();
    for(int i = 0; i < 100 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY()<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getX() ==Approx(-1 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(3 ).epsilon( 0.1 ));


    delete test;



  }
}


TEST_CASE( "Robot goto mouse sensor", "test move method on mouse sensor" ) {

  auto devices = getMouseDevices();

  int sensorPinLeft = 12;
  int sensorPinRight = 11;

  int forwardLeftpin = 9;
  int forwardRightpin = 10;

  int backwardLeftpin = 6;
  int backwardRightpin = 5;

  //Disable so that function that has not been defined is not called
  sensorPin[forwardLeftpin] = -1;
  sensorPin[backwardLeftpin] = -1;
  //Have to tell the system how many ticks are on each of the wheel sensors
  ticks[sensorPinLeft] = 20;

  sensorPin[forwardRightpin] = -1;
  sensorPin[backwardRightpin] = -1;
  ticks[sensorPinRight] = 20;
  double wheelDiameter = 0.065;
  double diameterBetweenWheelSensors = 0.155;

  hardware_mouse_event[forwardLeftpin] = devices[0];
  hardware_mouse_event[backwardLeftpin] = devices[0];

  hardware_mouse_event[forwardRightpin] = devices[1];
  hardware_mouse_event[backwardRightpin] = devices[1];

  Robot *test = new Robot(
    diameterBetweenWheelSensors ,
    new Wheel(
      forwardLeftpin,
      backwardLeftpin,
      new MouseSensor(devices[0],wheelDiameter)),
    new Wheel(
      forwardRightpin,
      backwardRightpin,
      new MouseSensor(devices[1],wheelDiameter))
    );
  SECTION("Test goto"){
    test->start();
    test->goTo(1,1);

    REQUIRE(test->getTargetX() == 1);
    REQUIRE(test->getTargetY() == 1);

    for(int i = 0; i < 50 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY() <<value["state"]<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    test->stop();

    REQUIRE(test->getX() ==Approx(1 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(1 ).epsilon( 0.1 ));

    test->start();
    test->goTo(4,7);
    REQUIRE(test->getTargetX() == 4);
    REQUIRE(test->getTargetY() == 7);
    test->start();
    for(int i = 0; i < 100 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY() <<value["state"]<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getX() ==Approx(4 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(7 ).epsilon( 0.1 ));

    test->goTo(-4,-3);
    REQUIRE(test->getTargetX() == -4);
    REQUIRE(test->getTargetY() == -3);

    for(int i = 0; i < 160 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY() <<value["state"]<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getX() ==Approx(-4 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(-3 ).epsilon( 0.1 ));


    delete test;



  }

  SECTION("Test targers"){
    test->goTo(1,1);
    test->goTo(2,-7);
    test->goTo(-1,3);
    REQUIRE(test->getTargetX() == 1);
    REQUIRE(test->getTargetY() == 1);
    test->start();
    for(int i = 0; i < 250 ; i++) {
      Json::Value value = test->getJSON();
      std::cout <<"goto robot at x:" <<test->getX()<<" y at:"<<test->getY()<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    REQUIRE(test->getX() ==Approx(-1 ).epsilon( 0.1 ));
    REQUIRE(test->getY() ==Approx(3 ).epsilon( 0.1 ));


    delete test;



  }
}
