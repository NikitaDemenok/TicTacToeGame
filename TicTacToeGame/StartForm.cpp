#include "StartForm.h"
#include "GameForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	TicTacToegame::StartForm form;
	Application::Run(% form);
}

TicTacToegame::StartForm::StartForm(void)
{
	InitializeComponent();

	//Функция, предназначенная для инициализации звуков
	sound = gcnew System::Media::SoundPlayer("1.wav");
}

System::Void TicTacToegame::StartForm::оПрограммеToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	sound->Play();//Функция, предназначенная для воспроизведения звука
	MessageBox::Show("Игра \"Крестики-Нолики\".", "Информация о программе!");
}

System::Void TicTacToegame::StartForm::выходToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	sound->Play();//воспроизведение звука
	Application::Exit();
}

System::Void TicTacToegame::StartForm::buttonStartGame_Click(System::Object^ sender, System::EventArgs^ e)
{
	sound->Play();//воспроизведение звука

	//Функция, предназначенная для считывания данных
	int indexGameMode = listBoxGameMode->SelectedIndex;
	if (indexGameMode == -1) {
		MessageBox::Show("Выберите режим игры!", "Внимание!");
		return;
	}

	int size;
	size = Convert::ToInt32(numericUpDownSizeMap->Value);

	numericUpDownLength->Maximum = size;
	int length = Convert::ToInt32(numericUpDownLength->Value);

	//Функция, предназначенная для создания формы
	GameForm^ form = gcnew GameForm();
	form->rows = size;
	form->columns = size;
	form->length = length;
	form->selectedGameMode = indexGameMode;
	form->Show();
	this->Hide();
}
