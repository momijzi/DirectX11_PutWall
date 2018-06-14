#include "App.hpp"

int MAIN()
{
				GameScene gameScene;

				while (App::Refresh())
				{
								gameScene.SceneManager();
				}
				return 0;
}
