#include "EngineInterface.h"

namespace GE {
	extern Coordinator gCoordinator;
	static bool selection[MAX_ENTITIES];

	void GE::EngineInterface::EntityRaycast(float mouse_x, float mouse_y, float w, float h)
	{
		// Convert screen coordinates to world coordinates
		Vec3f worldCoords = camera->ScreenToWorld(mouse_x, mouse_y, w, h);

		Vec3f direction = camera->ComputeRayDirection(glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z));
		PxVec3 dir(direction.x, direction.y, direction.z);
		// Perform raycasting
		PxRaycastBuffer hit = physicsSystem->RayCastToObject(PxVec3(worldCoords.x, worldCoords.y, worldCoords.z), dir, 100.0f);
		/*std::cout << "-originx:" << worldCoords.x << "-ooriginy:" << worldCoords.y << "-ooriginz:" << worldCoords.z
			<< "-dirx:" << dir.x << "-diry:" << dir.y << "-dirz:" << dir.z << "-clookx:" << camera ->camTarget().x << "-clooky:"<< camera->camTarget().y << "-clookz:" << camera->camTarget().z ;
		auto cam = camera->getPos();
		auto obj = gCoordinator.GetComponent<Transform>(1);
		auto testdir = Vec3f(cam.x,cam.y,cam.z) - obj.position;
		std::cout <<"-TdirX:" << testdir.x <<"-TdirY:" << testdir.y <<"-TdirZ:" << testdir.z << "    "; */

		// Get the entity from the raycast result
		currentSelected = physicsSystem->GetEntityFromBuffer(hit.block.actor);
	}

	static void ShowExampleMenuFile()
	{
		ImGui::MenuItem("", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent"))
		{
			ImGui::MenuItem("fish_hat.c");
			ImGui::MenuItem("fish_hat.inl");
			ImGui::MenuItem("fish_hat.h");
			if (ImGui::BeginMenu("More.."))
			{
				ImGui::MenuItem("Hello");
				ImGui::MenuItem("Sailor");
				if (ImGui::BeginMenu("Recurse.."))
				{
					ShowExampleMenuFile();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..")) {}
		ImGui::Separator();
		if (ImGui::BeginMenu("Options"))
		{
			static bool enabled = true;
			ImGui::MenuItem("Enabled", "", &enabled);
			ImGui::BeginChild("child", ImVec2(0, 60), true);
			for (int i = 0; i < 10; i++)
				ImGui::Text("Scrolling Text %d", i);
			ImGui::EndChild();
			static float f = 0.5f;
			static int n = 0;
			static bool b = true;
			ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
			ImGui::InputFloat("Input", &f, 0.1f);
			ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
			ImGui::Checkbox("Check", &b);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Colors"))
		{
			float sz = ImGui::GetTextLineHeight();
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
				ImGui::Dummy(ImVec2(sz, sz));
				ImGui::SameLine();
				ImGui::MenuItem(name);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Disabled", false)) // Disabled
		{
			IM_ASSERT(0);
		}
		if (ImGui::MenuItem("Checked", NULL, true)) {}

		bool shouldExit = false;

		if (ImGui::MenuItem("Exit", "Alt+F4", &shouldExit)) {
			if (shouldExit) {
				std::cout << "Exiting engine " << shouldExit;
			}
		}
		
	}
	
	void EngineInterface::Menu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			auto& asdaa = ImGui::GetStyle().FramePadding.y;
			sceneViewportHeight = ImGui::GetWindowContentRegionMax().y + asdaa;

			if (ImGui::BeginMenu("File"))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//ImGui::Begin("EngineMainMenu");
		//ImGui::SetWindowSize(ImVec2(windowWidth / 10, windowHeight), ImGuiCond_FirstUseEver);
		//// NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
		//// To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
		//// would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
		//if (ImGui::BeginMenuBar()) {
		//	ImGui::PushID("foo");
		//	ImGui::MenuItem("Menu item", "CTRL+M");
		//	if (ImGui::BeginMenu("Menu inside a regular window"))
		//	{
		//		ShowExampleMenuFile();
		//		ImGui::EndMenu();
		//	}
		//	ImGui::PopID();
		//	ImGui::Separator();
		//	ImGui::TreePop();
		//	ImGui::SameLine();
		//	if (ImGui::TreeNode("Menus inside a regular window"))
		//	{
		//		// NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
		//		// To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
		//		// would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
		//		ImGui::PushID("foo");
		//		ImGui::MenuItem("Menu item", "CTRL+M");
		//		if (ImGui::BeginMenu("Menu inside a regular window"))
		//		{
		//			ShowExampleMenuFile();
		//			ImGui::EndMenu();
		//		}
		//		ImGui::PopID();
		//		ImGui::Separator();
		//		ImGui::TreePop();
		//	}
		//	ImGui::EndMenuBar();
		//}
		//ImGui::End();
		/*

		if (ImGui::BeginMainMenuBar()) {
			ShowExampleMenuFile();
			ImGui::EndMainMenuBar();

		}*/
		//ImGui::Begin("Menu");
		//ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight / 10), ImGuiCond_FirstUseEver);
		//ImGui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
		//ImGui::Separator();
		//// NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
		//// To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
		//// would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
		//ImGui::MenuItem("Menu item", "CTRL+M");
		//
		//ImGui::Separator();
		//	if (ImGui::BeginMenu("Menu inside a regular window"))
		//	{
		//		ShowExampleMenuFile();
		//		ImGui::EndMenu();
		//	}
		//	ImGui::Separator();
		//
		//ImGui::End();
	}

	void EngineInterface::DisplayCurrentSellectedData(std::vector<Entity>& gameEntities)
	{
		ImGui::Begin("Inspector");
		ImGui::SetWindowSize(ImVec2(windowWidth / 10, windowHeight), ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos(ImVec2(0.0f, 19.0f));

		if (currentSelected != NULL)
		{
			bool hasRigidBody = gCoordinator.HasComponent<RigidBody>(currentSelected);
			bool hasModel = gCoordinator.HasComponent<Model>(currentSelected);
			bool hasCamera = gCoordinator.HasComponent<Camera>(currentSelected);
			bool hasPhysics = gCoordinator.HasComponent<Physics>(currentSelected);
			bool hasTransform = gCoordinator.HasComponent<Transform>(currentSelected);

			if (hasTransform) {
				Transform& transform = gCoordinator.GetComponent<Transform>(currentSelected);

				float* position[3]{ &transform.position.x,&transform.position.y,&transform.position.z };
				float* rotation[3]{ &transform.rotation.x,&transform.rotation.y,&transform.rotation.z };
				float* scale[3]{ &transform.scale.x, &transform.scale.y,&transform.scale.z };

				ImGui::MenuItem("Transform", "t", false, true);
				ImGui::InputFloat3(": Position", *position);
				ImGui::InputFloat3(": Rotation", *rotation);
				ImGui::InputFloat3(": Scale", *scale);

				ImGui::Separator();
			}

			if (hasRigidBody) {

				ImGui::Separator();
			}
			if (hasModel) {
				Model& entity = gCoordinator.GetComponent<Model>(currentSelected);
				ImGui::InputText("Model Name", &entity.name[0], entity.name.capacity());
				ImGui::Separator();
			}
			if (hasCamera) {

				ImGui::Separator();
			}
			if (hasPhysics) {

				ImGui::Separator();
			}

			//if (ImGui::BeginCombo("Transform Options", selectedTransformOption.c_str()))
			//{
			//	const char* transformOptions[] = { "Position", "Rotation", "Scale" };
			//	for (int i = 0; i < IM_ARRAYSIZE(transformOptions); i++)
			//	{
			//		bool isSelected = (selectedTransformOption == transformOptions[i]);
			//		if (ImGui::Selectable(transformOptions[i], isSelected))
			//		{
			//			selectedTransformOption = transformOptions[i];
			//			// Handle the selected option here (e.g., set a variable, trigger an action)
			//		}
			//		if (isSelected)
			//			ImGui::SetItemDefaultFocus();
			//	}
			//	ImGui::EndCombo();
			//}
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
			{
				//show_another_window = false;
			}
		}
		ImGui::End();

		ImGui::Begin("Hierarchy");
		float sizeX = windowWidth / 10;
		float sizeY = windowHeight;

		ImGui::SetWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos(ImVec2{ windowWidth - sizeX , 0 });
		ImGui::Separator();

		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();

		if (ImGui::TreeNode("Selection State: Single Selection"))
		{
			static int selected = -1;
			for (int n = 0; n < gameEntities.size(); n++)
			{
				char buf[32];
				sprintf_s(buf, "Object %d", n);
				if (ImGui::Selectable(buf, selected == n))
				{
					selected = n;
				}
			}
			if (currentSelected < 0)
				currentSelected = selected;

			ImGui::TreePop();
		}
		ImGui::End();
	}
}