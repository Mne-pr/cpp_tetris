#include "rendering.h"
#include "controller.h"
#include "notes.h"
#include "planeTable.h"

int main() {
	int rerun = 1;
	controller* con;

	while (rerun) {
		con = new controller();
		rerun = con->RunGame();
		delete(con);
	}

	return 0;
}

//여기서 게임이 끝났을 때 반환값을 확인하고, 반복여부를 결정해도 될듯?
