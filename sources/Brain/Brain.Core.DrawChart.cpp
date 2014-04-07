#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// График суммарного ритма
void Brain::Core::DrawChart() {
	int YSize = 40;
	int Y = 0;
	double Amax = 0.05;
	pic2 = gcnew System::Drawing::Bitmap(this->NSer, YSize);

	int i = 0;
	for (int k = this->CurPosA + 1; k < this->NSer; k++) {
		Y = (int)( this->AWave[k] * (YSize - 1) / Amax );
		if (Y >= YSize - 1) {
			Y = YSize - 1;
		}
		pic2->SetPixel(i, YSize - Y - 1, System::Drawing::Color::Black);
		i++;
	}

	for (int k = 0; k <= this->CurPosA; k++) {
		Y = (int)( this->AWave[k] * (YSize - 1) / Amax );
		if (Y >= YSize - 1) {
			Y = YSize - 1;
		}
		pic2->SetPixel(i, YSize - Y - 1, System::Drawing::Color::Black);
		i++;
	}
} // График суммарного ритма. DrawChart()