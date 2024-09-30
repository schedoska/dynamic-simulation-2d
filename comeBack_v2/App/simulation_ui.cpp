#include "simulation_ui.h"

simulation_ui::simulation_ui()
{
}

void simulation_ui::draw()
{
	ImGui::Begin("Simulation");

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.027, 0.741, 0.082, 1));
	ImGui::Button("Start");
	ImGui::PopStyleColor(1);
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.949, 0.188, 0.188, 1));
	ImGui::Button("Restart");
	ImGui::PopStyleColor(1);

	ImGui::SeparatorText("Simulation speed");
	float fps = 40.0;
	static float step = (1.0 / fps) * 1000.0; // in ms not seconds!
	ImGui::InputFloat("Time step", &step, 0.2f, 10.0f, "%.1f ms");
	step = std::max(step, 0.f);
	float norm_step = (1.0 / fps) * 1000.0;	// in ms
	float speed_ratio = (step / norm_step) * 100.0; // relative to normal speed for selected fps 
	if (ImGui::DragFloat("asd", &speed_ratio, 0.2, 0, 10e10, "%.1f %%")) {
		step = norm_step * speed_ratio / 100.0;
	}

	ImGui::SeparatorText("Joints");
	static int iterations = 10;
	ImGui::InputInt("Iterations", &iterations, 1, 10);
	iterations = std::max(iterations, 0);

	ImGui::End();
}
