#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Проекция по волновому туннелю
void Brain::Core::TMove() {
	if (this->cortex2 == nullptr) { // Ничего не делать, если туннель не создан
		return;
	}

	for (int ix = 0; ix < this->TD; ix++) { // TD - размеры туннеля
		for (int iy = 0; iy < this->TD; iy++) {
			// Координаты угла исходящего туннеля TX1, TY1
			// Координаты угла входящего туннеля TX2, TY2
			this->cortex2->Cortex[ix + this->TX2, iy + this->TY2].Status = this->Cortex[ix + this->TX1, iy + this->TY1].Status;
			this->cortex2->CortexA[ix + this->TX2, iy + this->TY2, this->cortex2->CurLayer] = this->CortexA[ix + this->TX1, iy + this->TY1, this->CurLayer];
			this->cortex2->Cortex[ix + this->TX2, iy + this->TY2].T = this->Cortex[ix + this->TX1, iy + this->TY1].T;
		}
	}
	
	this->cortex2->Wave();
} // Проекция по волновому туннелю. TMove()