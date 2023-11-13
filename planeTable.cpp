#include "planeTable.h"

using namespace std;
std::stringstream ss;

// 판 클래스

planeTable::planeTable() { 
	init();
}

planeTable::planeTable(rendering* r) { render = r; init(); }

planeTable::~planeTable() {
}


void planeTable::init() {
	for (int i = 0; i < PLANEH; i++) {
		for (int j = 0; j < PLANEW; j++) {
			movTable[i][j] = stockTable[i][j] = 0;
		}
	}
	score = gameover = 0;
}

void planeTable::clearMovTable() {
	for (int i = 0; i < PLANEH; i++) {
		for (int j = 0; j < PLANEW; j++) {
			movTable[i][j] = 0;
		}
	}
}

void planeTable::stockNote(note* n) {
	for (int i = 0; i < n->sH; i++) {
		for (int j = 0; j < n->sW; j++) {
			if (n->shape[i][j] == 1) {
				if (n->lty + i <= 3) gameover = 1;
				stockTable[n->lty + i][n->ltx + j] = 1;
			}
		}
	}
}

void planeTable::movNote(note* n) {
	for (int i = 0; i < n->sH; i++) {
		for (int j = 0; j < n->sW; j++) {
			movTable[n->lty + i][n->ltx + j] = n->shape[i][j];
		}
	}
}

int planeTable::checkLine() {
	int line = 0;
	for (int tt = 0; tt < PLANEH; tt++) {
		for (int i = PLANEH - 1; i >= 0; i--) {
			int stock = 0;
			for (int j = 0; j < PLANEW; j++) {
				if (stockTable[i][j] == 1) stock++;
			}
			if (stock == PLANEW) {
				for (int k = i - 1; k >= 0; k--) {
					for (int l = 0; l < PLANEW; l++) {
						stockTable[k + 1][l] = stockTable[k][l];
					}
				}
				score += PLANEW; line++;
			}
		}
	}
	if (line == 0) return 0;
	else return 1;
}

int planeTable::setMovTable(note* n) {
	// 겹치는지 검사. 겹치면 복구 후 stockTable에 추가한 뒤 신호반환
	n->go();
	for (int i = 0; i < n->sH; i++) {
		for (int j = 0; j < n->sW; j++) {
			if (n->shape[i][j] == 1 && stockTable[n->lty + i][n->ltx + j] == 1 || n->sH + n->lty > PLANEH) {
				if (n->command == DOWN) { n->ungo(); stockNote(n); return 1; }
				else { n->ungo(); break; }
			}
			if (n->ltx < 0 || n->ltx + n->sW > PLANEW) {
				n->ungo(); break;
			}

		}
	}

	// 겹치지 않으면 movTable에 추가
	movNote(n);
	return 0;
}

void planeTable::tablePrint() {
	for (int i = 0; i < PLANEW + 2; i++) { render->screenRender(L'▣', 0); } render->screenEndl();

	for (int i = 4; i < PLANEH; i++) {
		render->screenRender(L'▣', 0);
		for (int j = 0; j < PLANEW; j++) {

			if (movTable[i][j] == 1 || stockTable[i][j] == 1) render->screenRender(L'■', 0);
			else render->screenRender(L'′', 0);
		}
		render->screenRender(L'▣', 0); render->screenEndl();
	}

	for (int i = 0; i < PLANEW + 2; i++) { render->screenRender(L'▣', 0); } render->screenEndl();
}

void planeTable::scorePrint() {
	ss.str(""); ss << "score" << endl;
	render->screenRender(PLANEW * 2 + 5, 10, (char*)(ss.str().c_str()));

	ss.str(""); ss << score << endl;
	render->screenRender(PLANEW * 2 + 5, 11, (char*)(ss.str().c_str()));
}

void planeTable::nextPrint(note* n) {
	ss.str(""); ss << "next :" << endl;
	render->screenRender(PLANEW * 2 + 5, 0, (char*)(ss.str().c_str()));

	for (int i = 0; i < n->sH; i++) {
		for (int j = 0; j < n->sW; j++) {
			if (n->shape[i][j] == 0) continue;
			render->screenRender((PLANEW + j) * 2 + 6, i + 2, L'■', 0);
		}
	}
}
