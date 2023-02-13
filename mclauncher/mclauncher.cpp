#include "mclauncher.h"
#include "consolelauncher.h"

using namespace std;

int main(int argc, char** argv)
{

	ConsoleLauncher launcher;
	launcher.run(argc, argv);
	return 0;
}
