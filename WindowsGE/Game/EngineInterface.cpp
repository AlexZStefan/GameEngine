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
	//		std::cout << origin.x << " " << origin.y << " " << origin.z << " dir " << dir.x << " " << dir.y << " " << dir.z << "\n";

	void EngineInterface::DisplayCurrentSellectedData(float windowWidth, float windowHeight, std::vector<Entity>& gameEntities)
	{
		ImGui::SetWindowSize(ImVec2(windowWidth / 10, windowHeight), ImGuiCond_FirstUseEver);

		ImGui::Begin("Inspector");
		if (currentSelected != NULL)
		{
			ImGui::SetWindowPos(ImVec2{ 0,0 });
			bool hasRigidBody = gCoordinator.HasComponent<RigidBody>(currentSelected);
			bool hasModel = gCoordinator.HasComponent<Model>(currentSelected);
			bool hasCamera = gCoordinator.HasComponent<Camera>(currentSelected);
			bool hasPhysics = gCoordinator.HasComponent<Physics>(currentSelected);
			bool hasTransform = gCoordinator.HasComponent<Transform>(currentSelected);

			if (hasTransform) {
				Transform& transform = gCoordinator.GetComponent<Transform>(currentSelected);
				
				float *position[3]{ &transform.position.x,&transform.position.y,&transform.position .z};
				float *rotation[3]{ &transform.rotation.x,&transform.rotation.y,&transform.rotation.z};
				float *scale[3]{ &transform.scale.x, &transform.scale.y,&transform.scale.z};

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

			if(currentSelected < 0)
				currentSelected = selected;

			ImGui::TreePop();
		}
			ImGui::End();
	}
}