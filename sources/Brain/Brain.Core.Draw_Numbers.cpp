#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Рисование номеров входных аксонов
void Brain::Core::Draw_Numbers() {
	System::Drawing::SolidBrush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Yellow);
	System::Drawing::Font ^font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);

	for (int i = 0; i < this->NP; i++) {
		this->Gpic->DrawString(i.ToString(), font, brush, (float)this->PatSet[i].ix, (float)this->PatSet[i].iy);
	}
} // Рисование номеров входных аксонов. Draw_Numbers()