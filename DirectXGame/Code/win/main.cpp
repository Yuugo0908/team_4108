#include "MyGame.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Framework* game = new MyGame();

	game->Run();

	safe_delete(game);

	return 0;
}