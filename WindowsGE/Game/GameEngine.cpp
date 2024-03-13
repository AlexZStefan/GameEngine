#include "GameEngine.h"
namespace GE {

	Coordinator gCoordinator;
	EngineInterface engineInterface;

	GE::GameEngine::GameEngine(int setVSync)
	{
		setVsync = setVSync;
		objectManager = std::make_unique <ObjectManager>();
	}

	GE::GameEngine::~GameEngine()
	{
	}

	bool GE::GameEngine::init()
	{
		times = new Time();
		/* Initialize the SDL library */
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			fprintf(stderr,
				"Couldn't initialize SDL: %s\n", SDL_GetError());
			return false;
		}
		// Set OpenGL version profile
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		// Set type of profile which is core meaning modern OpenGL
		// no legacy features for backword compatibility
		// SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
		// SDL_GL_CONTEXT_PROFILE_CORE
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		// SDL_WINDOW_INPUT_GRABBED - DISABLE IF LINUX COMPATIBILITY ISSUES // |		SDL_WINDOW_INPUT_GRABBED
		window = SDL_CreateWindow("ZarEngine", 50, 50, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			std::cerr << "Unable to create window." << SDL_GetError() << std::endl;
			return false;
		}
		// Context represents OpenGL for program such as objects and drawing
		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr)
		{
			std::cerr << "SDL could not create GL context " << SDL_GetError() << std::endl;
			return false;
		}

		//SDL_ShowCursor(SDL_DISABLE);
		// GLEW binds latest extensions that can be used; default API from `97
		GLenum status = glewInit();
		if (status != GLEW_OK)
		{
			std::cerr << "Error initialising GLEW " << glewGetErrorString(status) << std::endl;
			return false;
		}
		if (SDL_GL_SetSwapInterval(setVsync) != 0)
		{
			std::cerr << "Unable to set VSync!" << SDL_GetError() << std::endl;
			return false;
		}
		// Setup ImGui binding
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplSdlGL3_Init(window);
		// Setup style
		ImGui::StyleColorsDark();
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// position, target, up, fov, ar, near & far clip
		main_cam = std::make_shared<Camera>(glm::vec3(.0f, .0f, 0.0f),
			glm::vec3(.0f, 0.0f, -10.0f),
			glm::vec3(.0f, 1.0f, .0f),
			60.0f, windowWidth / windowHeight, 0.1f, 10000.0f);

		InitialiseSystems();
		// Create shaders 
		initShaders();
		// Load up models
		initModels();
		// Create renderers 
		createRenderer();

		
		// create input system
		inputSystem = std::make_unique<InputSystem>(&input_event, main_cam, physicsSystem);
		std::cout << "SDL Init successful " << std::endl;
		/*
		// 0x00327eb4
		//uniform vec3 lightColour;
		//uniform vec3 lightPosition;
		//uniform vec3 viewPosition;
		shader_basic->SetUniform3i("lightColour", 1, 0, 0);
		shader_basic->SetUniform3i("lightPosition", 1, 1, 1);
		shader_basic->SetUniform3i("viewPosition", 1, 1, 1);*/

		/// <summary>
		/// TEST FROM HERE ON
		/// </summary>
		/// <returns></returns>
		/// 	
		//Test();
		AddModel();

		return true;
	}

	bool GE::GameEngine::initShaders() {
		shader_basic_normals = std::make_shared<Shader>();
		shader_basic_normals->loadShader("basic_vshader_normal.vert", "basic_fshader_normal.frag");
		std::cout << "basic shader id: " << shader_basic_normals->getProgramId() << std::endl;
		rendererSystem->shaders["shader_basic_normals"] = shader_basic_normals;

		shader_basic = std::make_shared<Shader>();
		shader_basic->loadShader("basic_vshader.vert", "basic_fshader.frag");
		std::cout << "basic shader id: " << shader_basic->getProgramId() << std::endl;
		rendererSystem->shaders["shader_basic"] = shader_basic;

		shader_basic_text = std::make_shared<Shader>();
		std::cout << "text shader id: " << shader_basic_text->getProgramId() << std::endl;
		shader_basic_text->loadShader("./resources/assets/shaders/basic_text_shader.vert",
			"./resources/assets/shaders/basic_text_shader.frag");
		rendererSystem->shaders["shader_basic_text"] = shader_basic_text;

		shader_basic_text->bind();
		shader_basic_text->SetUniform3f("Colour", 1, 1, 1);
		return true;
	}

	bool GE::GameEngine::InitialiseSystems() {
		SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);

		gCoordinator.Init();
		gCoordinator.RegisterComponent<Transform>();
		gCoordinator.RegisterComponent<Player>();
		gCoordinator.RegisterComponent<Model>();
		gCoordinator.RegisterComponent<Camera>();
		gCoordinator.RegisterComponent<Physics>();
		gCoordinator.RegisterComponent<RigidBody>();
		//gCoordinator.RegisterComponent<physx::Transform>();

		rendererSystem = gCoordinator.RegisterSystem<Renderer>();
		{
			Signature signature;
			signature.set(gCoordinator.GetComponentType<Transform>());
			signature.set(gCoordinator.GetComponentType<Model>());
			gCoordinator.SetSystemSignature<Renderer>(signature);
			rendererSystem->camera = main_cam;
		}

		physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
		{
			Signature signature;
			signature.set(gCoordinator.GetComponentType<Transform>());
			signature.set(gCoordinator.GetComponentType<Physics>());
			gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
			physicsSystem->StartUp();
		}
		
		engineInterface.EngineInterfaceInit(main_cam, physicsSystem);

		return true;  
	}

	

	bool GE::GameEngine::initModels() {
		game_textures;
		load_textures("./resources/assets/textures.txt");

		game_textures[ship_uv]->loadNormalMap("./resources/assets/textures/ship_uv.jpg");

		// Objects
		load_models("./resources/assets/models.txt");
		// set healthbar height 
		game_textures[health_bar]->setHeight(20);

		terrain = std::make_shared<TerrainGenerator>("./resources/assets/textures/hmap.jpg", 1, 1);
		blb = std::make_shared<Billboard>(game_textures[tree_billboard]);
		b_rainDrop = std::make_shared<Billboard>(game_textures[rain_drop]);
		b_grass = std::make_shared<Billboard>(game_textures[blb_grass]);
		b_cloud = std::make_shared<Billboard>(game_textures[blb_cloud]);
		b_moon = std::make_shared<Billboard>(game_textures[blb_moon]);

		LOD_tree = std::make_shared<Billboard>(game_textures[tree_billboard]);
		terrain->setMaterial(game_textures[terrain_texture]->getTexture());

		blb->transform.scale = { 10.0f, 10.0f, 0 };

		// Extra 
		//terrain->CreateFoliage(blb, 100, 20, 1);
		//terrain->CreateFoliage(b_grass, 100, 1, 0);

		//foliage = terrain->getFoliage();

		FPS = new FontRenderer();
		GUI = new GUIRenderer(windowWidth, windowHeight);

		GUI->addShader(shader_basic_text);
		FPS->loadFont("HEllo", "./resources/assets/fonts/arial.ttf", 100);

		player.maxStats = game_textures[health_bar]->getWidth();
		for (auto& m : game_models) {
			m->Init();
		}
		return true;
	}
#pragma region Tests
	// PHYSICS 
	void GameEngine::AddModel() {

		//Model terM = Model();
		//terM.loadFromFile("./resources/assets/models/ter.obj", 0);
		std::vector<PxVec3>* pxVerts = new std::vector<PxVec3>();
		Shared<Model> m = game_models[terrain_model];
		for (int i = 0; i < m->vertices.size(); i++) {
			PxVec3 pos;
			Vertex& v = m->vertices[i];
			pos.x = v.x;
			pos.y = v.y;
			pos.z = v.z;
			pxVerts->push_back(pos);
		}
		//physicsSystem->CreateTerrainMesh(pxVerts, m->vertices.size());
		/*Entity cart = objectManager->CreateObject();
		objectManager->AddComponent(cart, Model(shader_basic_normals));
		Transform& a = objectManager->AddComponent(cart, Transform{});*/

		auto eTerrain = gCoordinator.CreateEntity();
		{
			gCoordinator.AddComponent(eTerrain, Transform{});
			gCoordinator.AddComponent(eTerrain, Model{});
			gCoordinator.AddComponent(eTerrain, Physics{});
			auto& model = gCoordinator.GetComponent<Model>(eTerrain);
			auto& phy = gCoordinator.GetComponent<Physics>(eTerrain);
			auto& transform = gCoordinator.GetComponent<Transform>(eTerrain);

			transform.name = model.name;
			game_models[terrain_model]->setMaterial(game_textures[terrain_texture]->getTexture());
			model.SetModel(game_models[terrain_model]);
			model.transform.position = Vec3f(0, 0, 0);

			phy.actor = physicsSystem->CreateTriangleStaticMesh(&model);
			//physicsSystem->CreateTerrainMesh(pxVerts, m->vertices.size());
		}
		gameEntities.push_back(eTerrain);

		auto eShip = gCoordinator.CreateEntity();
		{
			gCoordinator.AddComponent(eShip, Transform{});
			gCoordinator.AddComponent(eShip, Model{});
			gCoordinator.AddComponent(eShip, Physics{});
			auto& model = gCoordinator.GetComponent<Model>(eShip);
			model.SetModel(game_models[ship]);
			model.transform.position = Vec3f(0, 35, 0);

			// generate physics terrain
			auto& phy = gCoordinator.GetComponent<Physics>(eShip);
			model.transform.position = gCoordinator.GetComponent<Transform>(eShip).position;
			phy.actor = physicsSystem->CreateTriangleDynamicMesh(&model);
			phy.isKinematic = true;
			/*
			phy.AddShape(physicsSystem->m_PhysX->createShape(physx::PxBoxGeometry(1, 1, 1), *physicsSystem->m_PxDefaultMaterial));
			phy.AddActor(physicsSystem->m_PhysX->createRigidDynamic(localTm));
			phy.actor->attachShape(*phy.shape);
			physicsSystem->m_PxScene->addActor(*phy.actor);
			*/
		}
		gameEntities.push_back(eShip);

		auto eShip2 = gCoordinator.CreateEntity();
		{
			gCoordinator.AddComponent(eShip2, Transform{});
			gCoordinator.AddComponent(eShip2, Model{});
			gCoordinator.AddComponent(eShip2, Physics{});
			auto& model = gCoordinator.GetComponent<Model>(eShip2);
			model.SetModel(game_models[ship]);
			model.transform.position = Vec3f(0, 25, 0);

			// generate physics terrain
			auto& phy = gCoordinator.GetComponent<Physics>(eShip2);
			model.transform.position = gCoordinator.GetComponent<Transform>(eShip2).position;
			phy.actor = physicsSystem->CreateTriangleDynamicMesh(&model);

			/*
			phy.AddShape(physicsSystem->m_PhysX->createShape(physx::PxBoxGeometry(1, 1, 1), *physicsSystem->m_PxDefaultMaterial));
			phy.AddActor(physicsSystem->m_PhysX->createRigidDynamic(localTm));
			phy.actor->attachShape(*phy.shape);
			physicsSystem->m_PxScene->addActor(*phy.actor);
			*/
		}
		gameEntities.push_back(eShip2);
	}

	void GameEngine::Test() {
		//max entities are 500 and set in ECH.h
		std::vector<Entity> entities = std::vector<Entity>(450);
		//CHANGE THE ENTITIES TO 1 
		for (auto& entity : entities) {
			entity = gCoordinator.CreateEntity();
			gCoordinator.AddComponent(entity, Transform{});
			gCoordinator.AddComponent(entity, Model{});
			//gCoordinator.GetComponent<Model>(user).setMaterial(game_textures[ship_uv]->getTexture());
			auto& trans = gCoordinator.GetComponent<Transform>(entity);
			auto& m = gCoordinator.GetComponent<Model>(entity);
			m.SetModel(game_models[ship]);

			m.transform.position.x = 0;
			m.transform.position.z = 0;
			m.transform.position.y = 0;
			m.transform.scale = Vec3f(1, 1, 1);

			//gCoordinator.SetSystemSignature<Renderer>(entity);
			trans.rotation = { .0f, 0, 0.f };
			trans.position = { 0.0f, 0, 0.0f };
		}
	}
#pragma endregion

	void GameEngine::createRenderer()
	{
		//ft/bk/up/dn/r/l
		skyBoxRenderer = std::make_unique<SkyboxRenderer>(
			"./resources/assets/textures/cloud_ft.jpg", //1 
			"./resources/assets/textures/cloud_bk.jpg", //4 
			"./resources/assets/textures/cloud_up.jpg",
			"./resources/assets/textures/cloud_dn.jpg",
			"./resources/assets/textures/cloud_rt.jpg", //3
			"./resources/assets/textures/cloud_lf.jpg" // 2
			);

		rend_billboard = std::make_unique<BillboardRenderer>("billboard.vert", "billboard.frag");
		rend_billboard->loadVA();

		Vec3f rainScale = Vec3f(1, 1, 1);
		rend_particles = std::make_unique<ParticleRenderer>(b_rainDrop, main_cam, rainScale, 1, 1000);
		rend_particles->loadVA();

		Vec3f cloudScale = Vec3f(200, 200, 100);
		clouds_particles = std::make_unique<ParticleRenderer>(b_cloud, main_cam, cloudScale, 250, 50);
		clouds_particles->loadVA();

		//game_renderers.push_back(std::make_unique<Renderer>(terrain, shader_basic));
		//game_renderers.push_back(std::make_unique<Renderer>(game_models[ship], shader_basic));
		/*game_renderers.push_back(std::make_unique<Renderer>(game_models[roman_warriors_blue], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[roman_warriors_red], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[sun], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[eagle], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[pavilion], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[gtemple], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[wood_cart], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[horse], shader_basic));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[rock], shader_basic_normals));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[lowp_tree], shader_basic));*/
		// set the LOD of the renderer 
		//LOD_tree->transform.scale = Vec3f(15, 15, 0);
		/*game_renderers[game_renderers.size() - 1]->setBillboardRend(true);
		game_renderers[game_renderers.size() - 1]->setBillboard(LOD_tree);*/

		/*game_renderers.push_back(std::make_unique<Renderer>(game_models[new_cart], shader_basic_normals));
		game_renderers.push_back(std::make_unique<Renderer>(game_models[wood], shader_basic_normals));*/
	}

	bool GE::GameEngine::keep_running()
	{
		SDL_PumpEvents();

		if (SDL_PeepEvents(&input_event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT))
		{
			return false;
		}


		return true;
	}

	int time()
	{
		int tick = SDL_GetTicks();

		return tick;
	}

	Vec3f rotateAround(Vec3f obj, float orX, float orY, float orZ, float speed) {
		// rotated around origin orX - based on the object position
		float radians = (float)(speed)*M_PI / 180;

		float cosRad = cos(radians);
		float sinRad = sin(radians);

		float pX = obj.x - orX;
		float pY = obj.y - orY;
		float pZ = obj.z - orZ;
		// rotate around Z
		if (orZ == 0)
		{
			float x = (cosRad * pX) -
				(sinRad * pY);

			float y = (sinRad * pX) +
				(cosRad * pY);

			pX = x + orX;
			pY = y + orY;

			return Vec3f{ pX,pY,obj.z };
		}
		// rotate around X
		if (orX == 0)
		{
			float z = (cosRad * pZ) -
				(sinRad * pY);

			float y = (sinRad * pZ) +
				(cosRad * pY);

			pZ = z + orX;
			pY = y + orY;

			return Vec3f{ obj.x,pY,pZ };
		}

		// rotate around Y
		if (orZ > 0) {


			float x = (sinRad * pZ) +
				(cosRad * pX);

			float z = (cosRad * pZ) -
				(sinRad * pX);

			pX = x + orX;
			pZ = z + orZ;

			return Vec3f{ pX,obj.y,pZ };
		}
	}

	void GE::GameEngine::update(float dt)
	{
		inputSystem->Update(window, dt);
		
		float camX = main_cam->getPosX();
		float camY = main_cam->getPosY();
		float camZ = main_cam->getPosZ();
		glm::vec3 camPosition = main_cam->getPos();
		glm::vec3 lookdir = main_cam->getTarget();

		//std::cout << "camX " << camX << "/ camY " << camY << "/ camZ " << camZ << "  ";
		//std::cout << "lookdir x " << lookdir.x << "/ lookdir Y " << lookdir.y << "/ lookdir Z " << lookdir.z << "\n";

		// update shader lightPosition acording to sun rotation 
		/*Vec3f sunPosition = rotateAround(game_models[sun]->transform.position, 500, 1, 0, 0.1);
		game_models[sun]->transform.position = sunPosition;

		b_moon->transform.position = Vec3f(sunPosition.x - 1000, sunPosition.y, sunPosition.z - 1100);
		b_moon->transform.scale = Vec3f(100, 100, 5);*/
		/*shader_basic->SetUniform3f("lightPosition", sunPosition.x, sunPosition.y, sunPosition.z);
		shader_basic_normals->SetUniform3f("lightPosition", sunPosition.x, sunPosition.y, sunPosition.z);*/

		shader_basic->SetUniform3f("lightPosition", 0, 10, 0);
		shader_basic_normals->SetUniform3f("lightPosition", 0, 10, 0);

		/*Vec3f eaglePosition = rotateAround(game_models[eagle]->transform.position, 200, 0, 200, 0.1);
		game_models[eagle]->transform.position = eaglePosition;*/

		//update the shader view position with camera location
		shader_basic->SetUniform3f("viewPosition", camPosition.x, camPosition.y, camPosition.z);
		shader_basic_normals->SetUniform3f("viewPosition", camPosition.x, camPosition.y, camPosition.z);

		//	for (auto& r : game_renderers)
			//	r->update(main_cam);

			//std::cout << gCoordinator.GetComponent<Transform>(entities[0]).position.x << "\n";

			//game_models[ship]->transform.position = { game_models[ship]->transform.position.x + 0.05f, 10.0f, game_models[ship]->transform.position.z + 0.05f };

			/*float rain_x = main_cam->getPosX() + (rand() % 200 - 100);
			float rain_y = main_cam->getPosY() + 20;
			float rain_z = main_cam->getPosZ() + (rand() % 200 - 100);

			rend_particles->Update(main_cam, Vec3f(rain_x, -20, rain_z), Vec3f(rain_x, rain_y, rain_z)
				, Vec3f(0, -1, 0));

			float cloud_x = 0;
			float cloud_y = 100;
			float cloud_z = (rand() % 1000);

			clouds_particles->Update(main_cam, Vec3f(1000, 100, 0), Vec3f(cloud_x, 100, cloud_z)
				, Vec3f(1, 0, 0));*/
	}

	float light = 0;
	int lightningTime = 0;

	Vec3f lightning = { 1,1,1 };
	void GE::GameEngine::draw(float dt)
	{
		light += 0.001;
		glClearColor(0.4f, 0.5f, 0.9, 1.f);
		// set depth and culling 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyBoxRenderer->draw(main_cam);

		glEnable(GL_DEPTH_TEST);
		lightningTime = times->getSeconds() % 30;

		shader_basic_normals->SetUniform3f("lightColour", 1, 1, 1);
		shader_basic->SetUniform3f("lightColour", 1, 1, 1);

		//if (10 < lightningTime && lightningTime < 12)
		//{
		//	Vec3f thunderL = { 100, 100,100 };
		//	shader_basic_normals->SetUniform3f("lightColour", thunderL.x, thunderL.y, thunderL.z);
		//	shader_basic->SetUniform3f("lightColour", thunderL.x, thunderL.y, thunderL.z);
		//}
		//else
		//{
		//	//// set the light color of the shader - can simulate day - night cycle
		//	shader_basic->SetUniform3f("lightColour", lightning.x, lightning.y, lightning.z);
		//	shader_basic_normals->SetUniform3f("lightColour", lightning.x, lightning.y, lightning.z);
		//}
		//for (auto& r : game_renderers)
		//{
		//	if (r->getModel() == game_models[wood]);
		//	float cam_x = main_cam->getPosX();
		//	float cam_y = main_cam->getPosY();
		//	float cam_z = main_cam->getPosZ();
		//	Vec3f model_pos = r->getModel()->transform.position;
		//	if (r->getModel()->hasNormalMap() == true)
		//	{
		//		shader_basic_normals->bind();
		//		r->draw(main_cam);
		//		shader_basic->bind();
		//		continue;
		//	}
		//	if (r->getBillboardRend() == true&&
		//		sqrt((cam_x - model_pos.x) * (cam_x - model_pos.x)
		//			+ (cam_z - model_pos.z) * (cam_z - model_pos.z)) > 200)
		//	{
		//		// calculate distance from object to camera 
		//		
		//			LOD_tree->transform.position = r->getModel()->transform.position;
		//			rend_billboard->draw(r->getBillboard(), main_cam);
		//	}
		//	else
		//		r->draw(main_cam);
		//} 
	//	physicsSystem->Update(dt);
		rendererSystem->update(main_cam);
		physicsSystem->Update(dt);
		/*for (int i = 0; i < foliage.size()-1; i++) {
			rend_billboard->draw(foliage[i], main_cam);
		};*/
		//rend_billboard->draw(b_moon, main_cam);
		//rend_particles->draw(b_rainDrop, main_cam);
		//clouds_particles->draw(b_cloud, main_cam);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		shader_basic_text->SetUniform3f("Colour", 1, 0, 0);
		// glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO); //- predator vision 
		glBlendFunc(GL_SRC_COLOR, GL_ZERO);

		GUI->drawTexture(0, windowHeight - game_textures[health_bar]->getHeight(), game_textures[health_bar]);
		shader_basic_text->SetUniform3f("Colour", 0, 0, 1);
		GUI->drawTexture(windowWidth - game_textures[health_bar]->getWidth(), windowHeight - game_textures[health_bar]->getHeight(), game_textures[health_bar]);
		game_textures[health_bar]->setWidth(player.health);
		if (player.health <= 1.0f) player.health = player.maxStats;
		player.health--;
		glDisable(GL_BLEND);

		// debug Gui
		ImGui_ImplSdlGL3_NewFrame(window);
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
			ImGui::SliderFloat3("Rock->position", &game_models[rock]->transform.position.x, -100.0f, 1000);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("GreekRWarrior->position", &game_models[roman_warriors_red]->transform.position.x, -100.0f, 1000);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("GreekBWarrior->position", &game_models[roman_warriors_red]->transform.position.x, -100.0f, 1000);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("Tree->position", &game_models[lowp_tree]->transform.position.x, -100.0f, 1000);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("Tree->scale", &game_models[lowp_tree]->transform.scale.x, 1.0, 100);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("LightColor", &lightning.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("WoodCart Pos", &game_models[new_cart]->transform.position.x, 0.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("WoodCart Scale", &game_models[new_cart]->transform.scale.x, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
		engineInterface.DisplayCurrentSellectedData(windowWidth, windowHeight, gameEntities);

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);

		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
	}

	template <typename T>
	void GameEngine::AddGuiInspectorComponent(Entity entity) {
		Physics& component = gCoordinator.GetComponent<T>(entity);
		if (!component) return;
		switch (T)
		{
		case Transform:
			ImGui::InputFloat3(": Position", component.position.x, component.position.y, component.position.z);
			break;
		case Model:
			break;
		case Physics:
			ImGui::Checkbox("RigidBody", &component.isKinematic);
			ImGui::Checkbox("RigidBody", &component.isStatic);
			break;
		default:
			break;
		}
	}

	void doSomethingSecond(int howOften, int time, std::function<Vec3f>& func) {
		if (time % howOften == 0)
			func;
	}

	void setV(Vec3f& v, bool t) {
		if (t == true) v = Vec3f{ 100, 100, 100 };
		else v = Vec3f{ 1,1,1 };

	}

	void GE::GameEngine::exit()
	{
		SDL_DestroyWindow(window);
		//physicsSystem->ShutDown();
		window = nullptr;
		ImGui_ImplSdlGL3_Shutdown();
		ImGui::DestroyContext();
		physicsSystem->ShutDown();

		SDL_Quit();
	}

	void GE::GameEngine::set_window_title(const char* new_title)
	{
		SDL_SetWindowTitle(window, new_title);
	}

	void GameEngine::load_models(std::string filePath)
	{
		std::string line;

		std::vector<std::string> lines;

		std::ifstream in(filePath);

		// open file 
		if (!in.is_open()) {
			std::cout << "File was not opened: " << filePath << std::endl;
			return;
		}

		// if file open and not end of file 
		while (!in.eof()) {
			// store line 
			std::getline(in, line);
			// push into vector

			lines.push_back(line);
		}
		in.close();

		// for each line - load model and push into model into game_models
		for (std::string& line : lines) {

			Shared<Model> model = std::make_shared<Model>(shader_basic_normals);

			std::string name;
			for (auto& e : line) {
				if (e == '.') break;
				name.push_back(e);
			}

			bool flipUV = false;
			line.back() == '1' ? flipUV = true : flipUV = false;
			line.pop_back();

			model->loadFromFile(("./resources/assets/models/" + line).c_str(), flipUV);
			if (model == NULL) std::cout << "model not loaded :" + line << std::endl;

			model->name = name;
			// set default shader
	/*		model->mat->shader = rendererSystem->shaders["shader_basic_normals"];
			model->shader = rendererSystem->shaders["shader_basic_normals"];*/
			//model->Init();
			std::cout << "ADD DEFAULT TEXTURE when (GameEngine.loadModels) \n";
			model->setNormal(game_textures[ship_uv]->getNormalMap());
			model->setMaterial(game_textures[ship_uv]->getTexture());
			game_models.push_back(model);
		};
	}

	void display_info_message(const char* msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "BestGameEngine", msg, nullptr);
	}

	// not implemented
	void GameEngine::load_textures(std::string filePath)
	{
		int i = 0;

		std::string line;

		std::vector<std::string> lines;

		std::ifstream in(filePath);

		// open file 
		if (!in.is_open()) {
			std::cout << "File was not opened: " << filePath << std::endl;
			return;
		}

		// if file open and not end of file 
		while (!in.eof()) {
			// store line 
			std::getline(in, line);
			// push into vector
			lines.push_back(line);
		}
		in.close();

		// for each line - load model and push into model into game_models
		for (std::string& line : lines) {

			std::string name;
			for (auto& e : line) {
				if (e == '.') break;
				name.push_back(e);
			}

			Shared<Texture> texture = std::make_shared<Texture>(
				("./resources/assets/textures/" + line).c_str());

			if (texture == nullptr) std::cout << "model was not loaded " + line << std::endl;
			game_textures.push_back(texture);
		};
	}
}
