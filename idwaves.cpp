// idwaves.cpp : main project file.

#include "headers/stdafx.h"
#include "headers/FormCortexes/FormCortex1.h"

[System::STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	
	// Create the main window and run it
	System::Windows::Forms::Application::Run(gcnew IdWaves::FormCortex1());
	return 0;
}
