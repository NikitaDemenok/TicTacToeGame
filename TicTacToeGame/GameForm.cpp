#include "GameForm.h"
#include "StartForm.h"

enum GameMode {
	PvP = 0,//Функция, предназначенная для инициализации режима игры, в котором игрок играет против игрока
	PvE //Функция, предназначенная для инициализации режима игры, в котором игрок играет против искусственного интеллекта
} gameMode;

//Функция, предназначенная для инициализации общих данных
GameMap map;
Gamer player1;
Gamer player2;
GamerStatus currentPlayer;
Vector2 selectedCellPlayer;
//Функция, предназначенная для инициализации флажков
bool canPlay;
bool endGame;
bool sound;
//Функция, предназначенная для инициализации данных для AI
std::vector<Vector2> allMoves;//Функция, предназначенная для проверки всех возможных ходов
int currentMoves;//текущий ход, с которого начинать

System::Void TicTacToegame::GameForm::GameForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	//Функция, предназначенная для инициализации параметров
	if (selectedGameMode == 0) {
		gameMode = PvP;
	}
	else {
		gameMode = PvE;
	}

	//Функция, предназначенная для инициализации звуков
	soundClick = gcnew System::Media::SoundPlayer("1.wav");
	soundEndGame = gcnew System::Media::SoundPlayer("2.wav");
	sound = true;

	NewGame();
}

void TicTacToegame::GameForm::GameLogic()
{
	//Функция, предназначенная для проверки режима игры
	if (gameMode == PvE) {
		//Если очередь делать ход переходит к компьютеру, то
		if (currentPlayer == Computer) {
			//Делаем ход
			StepAI();
		}

		Update();
	}
}

void TicTacToegame::GameForm::Update()
{
	if (endGame)
		return;

	//Функция, предназначенная для проверки состояния игрового поля
	int state_game = map.CheckingWin();

	if (state_game == 1) {
		if (gameMode == PvP) {
			MessageBox::Show("Поздравляем Игрока1 с победой!!!", "Победа!");
		}
		else {
			MessageBox::Show("Поздравляем Игрока с победой!!!", "Победа!");
		}
		UpdateGameGrid();

		endGame = true;
	}
	else if (state_game == 2) {
		if (gameMode == PvP) {
			MessageBox::Show("Поздравляем Игрока2 с победой!!!", "Победа!");
		}
		else {
			MessageBox::Show("Поздравляем Компъютер с победой!!!", "Победа!");
		}
		UpdateGameGrid();

		endGame = true;
	}
	else if (state_game == 3) {
		MessageBox::Show("Победила дружба!!!", "Ничья!");
		UpdateGameGrid();

		endGame = true;
	}

	if (endGame) {
		if (sound) {
			soundEndGame->Play();
		}

		if (MessageBox::Show("Запустить новую игру?", "Внимание!", MessageBoxButtons::YesNo) == Windows::Forms::DialogResult::Yes) {
			NewGame();
		}
		return;
	}

	if (gameMode == PvE) {
		if (currentPlayer == Computer) {
			status->Text = "Ход: Игрока!";
			currentPlayer = Player;
			return;
		}
		else {
			status->Text = "Ход: Компьютера!";
			currentPlayer = Computer;

			GameLogic();
			UpdateGameGrid();
			return;
		}
	}
	else {
		if (currentPlayer == Player1) {
			status->Text = "Ход: 0!";
			currentPlayer = Player2;
		}
		else {
			status->Text = "Ход: X!";
			currentPlayer = Player1;
		}
	}

	GameLogic();
	UpdateGameGrid();
}

void TicTacToegame::GameForm::NewGame()
{
	//Функция, предназначенная для инициализации данных
	map.SetMap(rows, columns, length);
	rand = gcnew Random();
	endGame = false;

	CreateGameGrid(map.GetSize()); //Функция, предназначенная для создания игрового поля

	//Функция, предназначенная для инициализации всех возможных ходов для компьютера
	currentMoves = 0;
	allMoves.clear();
	Vector2 buf;
	for (int i = 0; i < map.GetSize().x; i++) {
		for (int j = 0; j < map.GetSize().x; j++) {
			buf.x = i;
			buf.y = j;

			allMoves.push_back(buf);
		}
	}

	//Функция, предназначенная для перемешивания всех возможных ходов несколько раз
	int num_mixing = rand->Next(1, 10);
	for (int i = 0; i < num_mixing; i++)
		std::random_shuffle(allMoves.begin(), allMoves.end());

	//Функция, предназначенная для проверки очередности и режима игры
	int state_gamer = rand->Next(1, 3);
	if (state_gamer == 1) {
		if (gameMode == PvE) {
			player1.SetField(Player, 1);
			player2.SetField(Computer, 2);

			status->Text = "Ход: Игрока!";

			currentPlayer = Player;
		}
		else {
			player1.SetField(Player1, 1);
			player2.SetField(Player2, 2);

			status->Text = "Ход: X!";

			currentPlayer = Player1;
		}
	}
	else if (state_gamer == 2) {
		if (gameMode == PvE) {
			player1.SetField(Player, 1);
			player2.SetField(Computer, 2);

			status->Text = "Ход: Компьютера!";

			currentPlayer = Computer;

			GameLogic();
			UpdateGameGrid();

		}
		else {
			player1.SetField(Player1, 1);
			player2.SetField(Player2, 2);

			status->Text = "Ход: 0!";

			currentPlayer = Player2;
		}
	}
	else {
		MessageBox::Show("Ошибка генерации первоначального выбора игрока!", "Ошибка!");
		return;
	}
}

void TicTacToegame::GameForm::StepAI()
{
	//Функция, предназначенная для проверки наличия ходов
	if (currentMoves < allMoves.size()) {
		//Функция, предназначенная для проверки текущего хода
		if (map.IsEmpty(allMoves[currentMoves])) {
			//Функция, предназначенная для проверки, свободно ли. Если свободно, делаем ход
			map.SetPosition(allMoves[currentMoves], player2.GetMark());
			currentMoves++;
		}
		else {
			//Если занято, то перемещаемся на следующий ход и вызываем рекурсивно данную функцию
			currentMoves++;
			StepAI();
		}
	}
}

/* Переписанные функции на языке С#
public void NewGame()
{
	//Функция, предназначенная для инициализации данных
	map.SetMap(rows, columns, length);
	rand = new Random();
	endGame = false;

	CreateGameGrid(map.GetSize()); //Функция, предназначенная для создания игрового поля

	//Функция, предназначенная для инициализации всех возможных ходов для компьютера
	currentMoves = 0;
	allMoves.Clear();
	Vector2 buf;
	for (int i = 0; i < map.GetSize().x; i++)
	{
		for (int j = 0; j < map.GetSize().x; j++)
		{
			buf.x = i;
			buf.y = j;

			allMoves.Add(buf);
		}
	}

	//Функция, предназначенная для перемешивания всех возможных ходов несколько раз
	int num_mixing = rand.Next(1, 10);
	for (int i = 0; i < num_mixing; i++)
		allMoves = allMoves.OrderBy(x = > rand.Next()).ToList();

	//Функция, предназначенная для проверки очередности и режима игры
	int state_gamer = rand.Next(1, 3);
	if (state_gamer == 1)
	{
		if (gameMode == PvE)
		{
			player1.SetField(Player, 1);
			player2.SetField(Computer, 2);

			status.Text = "Turn: Player!";

			currentPlayer = Player;
		}
		else
		{
			player1.SetField(Player1, 1);
			player2.SetField(Player2, 2);

			status.Text = "Turn: X!";

			currentPlayer = Player1;
		}
	}
	else if (state_gamer == 2)
	{
		if (gameMode == PvE)
		{
			player1.SetField(Player, 1);
			player2.SetField(Computer, 2);

			status.Text = "Turn: Computer!";

			currentPlayer = Computer;

			GameLogic();
			UpdateGameGrid();
		}
		else
		{
			player1.SetField(Player1, 1);
			player2.SetField(Player2, 2);

			status.Text = "Turn: 0!";

			currentPlayer = Player2;
		}
	}
	else
	{
		MessageBox.Show("Error generating initial player choice!", "Error!");
		return;
	}
}

public void StepAI()
{
	//Функция, предназначенная для проверки наличия ходов
	if (currentMoves < allMoves.Count)
	{
		//Функция, предназначенная для проверки текущего хода
		if (map.IsEmpty(allMoves[currentMoves]))
		{
			//Функция, предназначенная для проверки, свободно ли. Если свободно, делаем ход
			map.SetPosition(allMoves[currentMoves], player2.GetMark());
			currentMoves++;
		}
		else
		{
			//Если занято, то перемещаемся на следующий ход и вызываем рекурсивно данную функцию
			currentMoves++;
			StepAI();
		}
	}
}
*/
void TicTacToegame::GameForm::UpdateGameGrid() {
	for (int i = 0; i < map.GetSize().x; i++) {
		for (int j = 0; j < map.GetSize().y; j++) {
			if (!map.IsEmpty(i, j)) {
				if (map.GetValue(i, j) == 1) {
					dataGridView->Rows[i]->Cells[j]->Value = L"🪐";
				}
				else {
					dataGridView->Rows[i]->Cells[j]->Value = L"🌏";
				}
			}
		}
	}
}

void TicTacToegame::GameForm::CreateGameGrid(Vector2 size)
{
	//Функция, предназначенная для очистки таблицы
	dataGridView->Rows->Clear();
	dataGridView->Columns->Clear();

	//Функция, предназначенная для задачи стиля для шрифта
	System::Drawing::Font^ font = gcnew System::Drawing::Font("Microsoft Sans Serif", 14);
	dataGridView->DefaultCellStyle->Font = font;
	dataGridView->ColumnHeadersDefaultCellStyle->Font = font;
	dataGridView->RowHeadersDefaultCellStyle->Font = font;

	//Функция, предназначенная для задачи изображения для фона таблицы
	String^ imagePath = "og_og.jpg"; // Укажите путь к вашему изображению
	if (System::IO::File::Exists(imagePath)) {
		Bitmap^ backgroundImage = gcnew Bitmap(imagePath);
		dataGridView->BackgroundImage = backgroundImage;

		//Функция, предназначенная для задачи масштабирования изображения
		dataGridView->BackgroundImageLayout = ImageLayout::Stretch;  // или другие варианты: None, Tile, Center, Zoom
	}

	//Функция, предназначенная для создания столбцов
	for (int i = 0; i < size.x; i++) {
		DataGridViewButtonColumn^ column = gcnew DataGridViewButtonColumn();
		column->HeaderCell->Value = Convert::ToString(i + 1);
		column->Name = "column" + i;
		column->Width = 50;

		dataGridView->Columns->Add(column);
	}

	//Функция, предназначенная для создания строк
	for (int i = 0; i < size.y; i++) {
		dataGridView->Rows->Add();
		dataGridView->Rows[i]->HeaderCell->Value = Convert::ToString(i + 1);
		dataGridView->Rows[i]->Height = 50;

		//Функция, предназначенная для задачи сиреневого цвета для каждой ячейки строки
		for (int j = 0; j < size.x; j++) {
			dataGridView->Rows[i]->Cells[j]->Style->BackColor = Color::Lavender;
		}
	}
}

void TicTacToegame::GameForm::SetPositionPlayer(Vector2 cell)
{
	//Функция, предназначенная для проверки режима игры
	if (gameMode == PvE) {
		//Функция, предназначенная для проверки очередности игрока
		if (currentPlayer == Player) {
			//Функция, предназначенная для того, чтобы сделать ход
			if (!map.SetPosition(cell, player1.GetMark())) {
				MessageBox::Show("Данная позиция занята!", "Внимание!");
				return;
			}
		}
		else {
			MessageBox::Show("Еще не ваша очередь!", "Внимание!");
			return;
		}
	}
	else {
		//Функция, предназначенная для проверки очереди игрока
		if (currentPlayer == Player1) {
			//Функция, предназначенная для того, чтобы сделать ход
			if (!map.SetPosition(cell, player1.GetMark())) {
				MessageBox::Show("Данная позиция занята!", "Внимание!");
				return;
			}
		}
		else {
			//Функция, предназначенная для того, чтобы сделать ход
			if (!map.SetPosition(cell, player2.GetMark())) {
				MessageBox::Show("Данная позиция занята!", "Внимание!");
				return;
			}
		}
	}

	Update();
}

System::Void TicTacToegame::GameForm::dataGridView_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e)
{
	if (sound) {
		soundClick->Play();
	}
	//Функция, предназначенная для преобразования полученного объекта в таблицу
	auto senderGrid = (DataGridView^)sender;

	//Функция, предназначенная для запоминания индексов выбранной ячейки
	selectedCellPlayer.x = e->RowIndex;
	selectedCellPlayer.y = e->ColumnIndex;

	//Функция, предназначенная для того, чтобы сделать ход
	SetPositionPlayer(selectedCellPlayer);
}

System::Void TicTacToegame::GameForm::выходToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (sound) {
		soundClick->Play();
	}

	if (MessageBox::Show("Продолжить?", "Внимание!", MessageBoxButtons::YesNo) == Windows::Forms::DialogResult::Yes) {
		Application::Exit();
	}
}

System::Void TicTacToegame::GameForm::оПрограммеToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (sound) {
		soundClick->Play();
	}

	MessageBox::Show("Программа реализует игру «Крестики-нолики» стандартного типа для двух игроков и против компьютера. Реализована возможность выбора размерности поля и звукового сопровождения.", "Информация о программе!");
}

System::Void TicTacToegame::GameForm::новаяИграToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (sound) {
		soundClick->Play();
	}

	if (MessageBox::Show("Продолжить?", "Внимание!", MessageBoxButtons::YesNo) == Windows::Forms::DialogResult::Yes) {
		NewGame();
	}
}

System::Void TicTacToegame::GameForm::вернутьсяToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (sound) {
		soundClick->Play();
	}

	if (MessageBox::Show("Продолжить?", "Внимание!", MessageBoxButtons::YesNo) == Windows::Forms::DialogResult::Yes) {
		StartForm^ form = gcnew StartForm();
		form->Show();
		this->Hide();
	}
}

System::Void TicTacToegame::GameForm::вклВыклЗвукToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (sound) {
		soundClick->Play();
	}

	sound = !sound;
}