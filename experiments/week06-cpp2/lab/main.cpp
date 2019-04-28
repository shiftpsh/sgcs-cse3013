#include "LinkedList.h"
#include "Stack.h"

#include <iostream>
#include <string>

using namespace std;

// 함수의 선언
void prnMenu(void);

int main() {
    // 연결 리스트 테스트용 코드
	/*
    double dVal;
    string strVal;
    LinkedList<double> dList;
    LinkedList<string> strList;

    dList.Insert(3.14);
    dList.Insert(123456);
    dList.Insert(-0.987654);
    dList.Print();
    dList.Delete(dVal);
    cout << "삭제된 마지막 원소: " << dVal << endl;
    dList.Print();
    dList.Insert(777.777);
    dList.Print();
    dList.Delete(dVal);
    cout << "삭제된 마지막 원소: " << dVal << endl;
    dList.Delete(dVal);
    cout << "삭제된 마지막 원소: " << dVal << endl;
    dList.Print();
    dList.Delete(dVal);
    cout << "삭제된 마지막 원소: " << dVal << endl;
    dList.Print();

    strList.Insert("This");
    strList.Insert("is a");
    strList.Insert("Template");
    strList.Insert("Example");
    strList.Print();
    strList.Delete(strVal);
    cout << "삭제된 마지막 원소: " << strVal << endl;
    strList.Insert("Class");
    strList.Print();
*/
    // 스택 및 연결 리스트 테스트용 코드
    int mode, selectNumber, tmpItem;
    LinkedList<int> *p;
    bool flag = false;

    cout << "자료구조 선택(1: Stack, Other: Linked List): ";
    cin >> mode;

    // 기반 클래스의 포인터를 사용하여 기반 클래스 뿐만 아니라
    // 파생 클래스의 인스턴스 또한 접근할 수 있다.
    if (mode == 1)
        p = new Stack<int>(); // 정수를 저장하는 스택
    else
        p = new LinkedList<int>(); // 정수를 저장하는 연결 리스트

    // 처리 부분
    do {
        prnMenu();
        cin >> selectNumber;
        switch (selectNumber) {
        case 1:
            cout << "원하시는 값을 입력해주세요: ";
            cin >> tmpItem;
            p->Insert(tmpItem);
            cout << tmpItem << "가 삽입되었습니다." << endl;
            break;
        case 2:
            if (p->Delete(tmpItem) == true)
                cout << tmpItem << "가 삭제되었습니다." << endl;
            else
                cout << "비어있습니다. 삭제 실패" << endl;
            break;
        case 3:
            cout << "크기: " << p->GetSize() << endl;
            p->Print();
            break;
        case 4:
            flag = true;
            break;
        default:
            cout << "잘못 입력하셨습니다." << endl;
            break;
        }
        if (flag)
            break;
    } while (1);

    return 0;
}

void prnMenu() {
    cout << "*******************************************" << endl;
    cout << "* 1. 삽입    2. 삭제    3. 출력   4. 종료 *" << endl;
    cout << "*******************************************" << endl;
    cout << endl;
    cout << "원하시는 메뉴를 골라주세요: ";
}
