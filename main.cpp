#include "rendering.h"
#include "controller.h"
#include "notes.h"
#include "planeTable.h"

int main() {
	controller con;
	con.RunGame();
}


// 움직이는 조건 - 떨어지는 시간이 되었을 때 or 유저가 입력할 때
// 부딪히는 조건 - 객체의 위치를 바꾸었을 때, 겹친다면 취소하기
// 사라지는 조건 - 메인테이블에서 줄 하나가 완성되었을 때
// 출력하는 방법 - 판을 모두 0으로 초기화 -> 객체 하나씩 추가 시도


// 해야하는 것 
// 오류잡기..
// 
// 특정 스코어 이상 떨어지는 속도 증가
// 공중에서 많이 돌리면 점수 증가
// 블럭 색 지정하기
// 스페이스 바 입력 시 바로 떨어지게
// 
// 한 번에 없애기 -ok
// 게임오버 조건 -ok
// 