/*
TO DO:
	- antialiasing!
	- remake 2d rendering

	- check if single color texture work
	- change water reflection fbo resolution based on camera's distance from water
	- add possibility to add particle to the same particle group every frame
	- check default macros in particle systems (direction, spread)
	- remake 2d rendering system (Renderable/Sprite/Label as separate classes, not subclasses of Renderable2D)
	- single color texture issue

	- font rendering issue (maybe incorrect texture id loaded in shader)
	- check mipmapping

	+ add antialiasing to water
	+ below water bug, when you come to edge of water, you can see through the ground (see the sky)
	+ add 'uint' instead of 'unsigned int'
	+ add vector2 of uint's
	+ make texture atlas support for 2D
	+ organize whole code
	+ improve model loading (check for repeating indices, then combine it into one if it's the same vertex)
	+ add rotating to 2D/3D groups (using matrices)
	+ add support for rendering closest lights when there are too many important lights
	+ 2d camera zooming in and out
	+ change ParticleGroup std::vector of variables to std::vector of pointers in GraphicsScene3D class ///rethink
	+ think about adding for to BelowWater3D shader

	+ optimize 3d rendering with water (dont recalculate same things 3 times) /// idk what things (should've wrote it down when I remembered the things!)

*/




/* INCLUDES */
// Normal Includes
#include <iostream>
#include <vector>

// Gumi Gota Engine Includes
#include "../../Gumi Gota/src/GumiGota.h"

// Header Includes
#include "Game.h"
#include "GameScene.h"
#include "WalkingCamera3D.h"
#include "BulletObject.h"
#include "BulletManager.h"



/* DEFINITIONS */
#define FPS_TEST 0
#define FPS_TEST_LENGTH 150





#if 1
int main()
{
	/* Window Setup */
	gg::Window window("Gumi Gota Engine v0.0.1 - Tester", 1920, 1080);
	window.hideCursor();

	LOG("OpenGL version: " << std::endl << glGetString(GL_VERSION) << std::endl);


	/* TEST STUFF */
	gg::Model model1 = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/monkey.obj", "Gumi Gota/Engine Tester/res/textures/wood texture.jpg");
	gg::Renderable3D entity1(&model1, gg::vector3(0.0f));
	gg::Renderable3D entity2(&model1, gg::vector3(1.5f, 2.0f, 0.0f));
	gg::Renderable3D entity3(&model1, gg::vector3(-1.5f, 2.0f, 0.0f));

	gg::Model grassModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/grass.obj", "Gumi Gota/Engine Tester/res/textures/insane grass.png");
	gg::Renderable3D grassEntity(&grassModel, gg::vector3(-3.0f, 1.5f, 2.0f));
	grassModel.setHasFakeLighting(true);
	grassModel.setHasTransparency(true);
	grassEntity.setRotation(gg::vector3(gg::toRadians(-90.0f), 0.0f, 0.0f));

	gg::Model metalModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/logo cube.obj", "Gumi Gota/Engine Tester/res/textures/MetalPlate/Metal_plate_006_COLOR.jpg");
	gg::Renderable3D metalEntity(&metalModel, gg::vector3(4.0, 10.0f, -1.0f));

	gg::Model taModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/logo cube.obj", "Gumi Gota/Engine Tester/res/textures/best texture atlas ever.png");
	taModel.setTextureRows(2);
	gg::Renderable3D taEntity(&taModel, gg::vector3(10.0, 2.0f, 4.0f));
	taEntity.setTextureAtlasIndex(3);

	gg::Model donutModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/Wooden Donut Model.obj", "Gumi Gota/Engine Tester/res/textures/MetalPlate/Metal_plate_006_COLOR.jpg");
	gg::Renderable3D donutEntity(&donutModel, gg::vector3(-5.0, 3.0f, 16.0f));

	gg::Model groundModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/logo cube.obj", "Gumi Gota/Engine Tester/res/textures/MetalPlate/Metal_plate_006_COLOR.jpg");
	gg::Renderable3D groundEntity(&groundModel, gg::vector3(0.0, -34.0f, 0.0f));
	groundEntity.setScale(32.0f);

	gg::Model simpleLampModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/simple light model.obj", "Gumi Gota/Engine Tester/res/textures/simple lamp texture.png");
	gg::Renderable3D simpleLampEntity(&simpleLampModel, gg::vector3(8.0, 4.0f, 8.0f));
	gg::Light3D simpleLampLight(simpleLampEntity.getPosition(), gg::COLOR_GREEN, gg::vector3(1.0f, 0.01f, 0.005f));

	gg::Model bigDonutModel = gg::loadModelFromFile("Gumi Gota/Engine Tester/res/models/Wooden Donut Model.obj", "Gumi Gota/Engine Tester/res/textures/MetalPlate/Metal_plate_006_COLOR.jpg");
	gg::Renderable3D bigDonutEntity(&bigDonutModel, gg::vector3(-4.0, -5.0f, 4.0f));
	bigDonutEntity.setScale(36.0f);

	rx::BulletObject* bulletEntity = new rx::BulletObject(gg::vector3(0.0f, 5.0f, 0.0f), gg::vector3(0.0f), 0.0f);


	gg::Light3D moon(gg::vector3(-400000.0f, 250000.0f, -50000.0f), gg::vector3(0.855f / 1.4f, 0.969f / 1.2f, 1.0f), gg::vector3(1.0f, 0.0f, 0.0f));

	gg::CubeMap skybox(
		"Gumi Gota/Engine Tester/res/textures/hw_desertnight/desert_night_up.tga",
		"Gumi Gota/Engine Tester/res/textures/hw_desertnight/desert_night_dn.tga",
		"Gumi Gota/Engine Tester/res/textures/hw_desertnight/desert_night_lf.tga",
		"Gumi Gota/Engine Tester/res/textures/hw_desertnight/desert_night_rt.tga",
		"Gumi Gota/Engine Tester/res/textures/hw_desertnight/desert_night_ft.tga",
		"Gumi Gota/Engine Tester/res/textures/hw_desertnight/desert_night_bk.tga"
	);


	gg::Texture particleTexture1("Gumi Gota/Engine Tester/res/textures/particle texture atlas 2.png", 4, gg::LINEAR, gg::CLAMP_TO_EDGE, false);
	gg::Texture particleTexture2("Gumi Gota/Engine Tester/res/textures/particle texture atlas.png", 8, gg::LINEAR, gg::CLAMP_TO_EDGE, false);
	gg::Texture particleTexture3("Gumi Gota/Engine Tester/res/textures/particle1.png", gg::LINEAR, gg::CLAMP_TO_EDGE, false);
	
	gg::DirectionParticleSystem ps1(gg::vector3(0.0f, 20.0f, 0.0f), &particleTexture1);
	ps1.setUsingAdditiveBlending(true);
	ps1.setGravity(16.0f);
	ps1.setLifetime(2.0f);
	ps1.setLifetimeError(0.3f);
	ps1.setDirectionError(gg::vector3(4.0f, 2.0f, 4.0f));

	gg::DirectionParticleSystem ps2(gg::vector3(0.0f, 30.0f, 0.0f), &particleTexture2);
	///ps2.setEndingTextureAtlasIndex(31);
	ps2.setGravity(16.0f);
	ps2.setLifetime(3.0f);
	ps2.setLifetimeError(0.3f);
	ps2.setDirectionError(gg::vector3(4.0f, 2.0f, 4.0f));

	gg::SpreadParticleSystem ps3(&particleTexture1);
	ps3.setUsingAdditiveBlending(true);
	ps3.setScale(1.0f);
	ps3.setSpeed(50.0f);
	ps3.setSpeedError(10.0f);
	ps3.setLifetime(0.1f);
	ps3.setLifetimeError(0.1f);


	gg::WaterTile myWater(gg::vector3(0.0f, 0.0f, 0.0f), 32.0f);
	myWater.setTilingFactor(2.0f);
	myWater.setWaveStrength(0.1f);


	gg::Renderer3D rr(&window);
	rr.setSkyColor(gg::Color(0.098f, 0.149f, 0.271f));

	rr.addRenderable(&entity1);
	rr.addRenderable(&entity2);
	rr.addRenderable(&entity3);
	rr.addRenderable(&grassEntity);
	rr.addRenderable(&metalEntity);
	rr.addRenderable(&taEntity);
	rr.addRenderable(&donutEntity);
	rr.addRenderable(&groundEntity);
	rr.addRenderable(&simpleLampEntity);
	rr.addRenderable(&bigDonutEntity);
	rr.addRenderable(bulletEntity);

	rr.addLight(&moon, true);
	rr.addLight(&simpleLampLight);
	rr.setSkybox(&skybox);
	rr.addWaterTile(&myWater);

	rr.reserveParticleGroups(300);

	///rr.setReflectLightOnWater(false);
	///rr.setReflectSceneOnWater(false);


	gg::FlyingCamera3D cam(gg::vector3(2.5f, 2.5f, 6.0f), window.getAspectRatio(), &window);
	cam.setFOV(gg::toRadians(90.0f));
	cam.setPitch(gg::toRadians(-4.0f));
	cam.setYaw(gg::toRadians(30.0f));



	gg::PhysicsScene3D physicsScene;
	physicsScene.addCamera(&cam);


	gg::Layer2D textLayer(window);
	gg::Font testFont("Gumi Gota/Engine Tester/res/fonts/test font.ttf", 128.0f);
	gg::Label2D testText("text", gg::vector2(400.0f, 400.0f), gg::vector2(500.0f, 500.0f), &testFont);
	textLayer.addLabel(&testText);

	
	rx::BulletManager bulletManager(&rr, &physicsScene);


	gg::Timer fpsTimer; //used for fps calculating
	gg::Timer secondTimer; //used for fps calculating

	float totalFPS = 0.0f;
	unsigned int fpsSeconds = 0;


	gg::Timer deltaTimeTimer;





	unsigned int refleX = 1280;
	unsigned int refleY = 720;
	unsigned int refraX = 1280;
	unsigned int refraY = 720;


	/* APPLICATION LOOP */
	while (!window.closed())
	{
		// Delta Time Calculations
		float deltaTime = deltaTimeTimer.elapsed();
		///std::cout << deltaTime << std::endl;
		deltaTimeTimer.reset();

		if (deltaTime > 1.0f / 20.0f)
			deltaTime = 1.0f / 20.0f;
		

		fpsTimer.reset(); //used for fps calculating

		/* Clearing */
		window.clear();

		/* Updating */
		///rr.updateLights(cam.getPosition());
		grassEntity.rotate(gg::vector3(0.0f, 0.0f, 0.001f));

		// Input Handling
		{
			// Quit Control
			if (window.isKeyPressed(GLFW_KEY_ESCAPE))
			{
				window.close();
			}

			// Particle Controls
			if (window.isKeyClicked(GLFW_KEY_R))
			{
				rr.addParticleGroup(ps2.generateParticles(donutEntity.getPosition(), 100));
			}

			if (window.isKeyClicked(GLFW_KEY_E))
			{
				rr.addParticleGroup(ps1.generateParticles(donutEntity.getPosition(), 100));
			}

			if (window.isKeyPressed(GLFW_KEY_Y))
			{
				rr.addParticleGroup(ps2.generateParticles(donutEntity.getPosition(), 1000));
			}

			// Water FX Controls
			if (window.isKeyPressed(GLFW_KEY_F))
				myWater.setMovementSpeed(myWater.getMovementSpeed() + 0.001f);

			if (window.isKeyPressed(GLFW_KEY_G))
				myWater.setMovementSpeed(myWater.getMovementSpeed() - 0.001f);

			if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1))
				myWater.setWaveStrength(myWater.getWaveStrength() + 0.002f);

			if (window.isButtonPressed(GLFW_MOUSE_BUTTON_2))
				myWater.setWaveStrength(myWater.getWaveStrength() - 0.002f);

			if (window.isButtonPressed(GLFW_MOUSE_BUTTON_4))
				myWater.setTilingFactor(myWater.getTilingFactor() - 0.005f);

			if (window.isButtonPressed(GLFW_MOUSE_BUTTON_5))
				myWater.setTilingFactor(myWater.getTilingFactor() + 0.005f);

			// Explosion Controls
			if (window.isKeyClicked(GLFW_KEY_T))
			{
				physicsScene.explosion(gg::vector3(0.0f, -1.0f, 0.0f), gg::vector2(400.0f, 0.0f));
				rr.addParticleGroup(ps3.generateParticles(gg::vector3(0.0f, 0.0f, 0.0f), 1000));
			}

			// Bullet Control
			if (window.isKeyClicked(GLFW_KEY_X))
				bulletManager.createBullet(cam);

			// The Bullet Rotating Controls
			if (window.isKeyPressed(GLFW_KEY_C))
				bulletEntity->rotate(gg::vector3(0.01f, 0.0f, 0.0f));

			if (window.isKeyPressed(GLFW_KEY_V))
				bulletEntity->rotate(gg::vector3(0.0f, 0.01f, 0.0f));

			if (window.isKeyPressed(GLFW_KEY_B))
				bulletEntity->rotate(gg::vector3(0.0f, 0.0f, 0.01f));
		}


		bulletManager.update(deltaTime);
		rr.update(cam, deltaTime);
		physicsScene.update(deltaTime);

		myWater.update(deltaTime);

		/* Rendering */
		rr.render(cam);
		textLayer.render();

		/* Drawing */
		window.update();

		// FPS Calculations
		if (secondTimer.elapsed() >= 1.0f)
		{
			float fps = 1.0f / fpsTimer.elapsed();
			//std::cout << "[Info] FPS: " << fps << std::endl; //fps calculating and printing
			secondTimer.reset();

			if (FPS_TEST && fpsSeconds < FPS_TEST_LENGTH)
			{
				totalFPS += fps;

				fpsSeconds++;

				float averageFPS = totalFPS / fpsSeconds;
				std::cout << "[Info] FPS Test - Average FPS: " << averageFPS << std::endl;
			}
			else if (FPS_TEST && fpsSeconds == FPS_TEST_LENGTH)
			{
				float averageFPS = totalFPS / fpsSeconds;
				std::cout << "[Info] FPS Test - Final Average FPS: " << averageFPS << std::endl;
				system("pause");
			}
		}
	}


	return 0;
}
#endif