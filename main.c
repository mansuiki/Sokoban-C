#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

char map[5][30][30]; // load_map 에 temp 변수를 가공하여 저장하는 변수
char nowPlayMap[30][30] =  {'\0', }; // 현재 플레이하고 있는 맵을 저장하는 변수
int current_player_pos[2]; // 플레이어의 위치를 저장하는 변수
int current_goals = 0; //목표지점의 개수
int current_map_no;
_Bool check_error = 0, is_undoing;
char name[10] = {'\0'}; //사용자 이름을 받는 변수
int move_count=0; // 순위표에서 사용할 변수

_Bool is_box_moved = 0;

char cmd_history[5] = {'\0'}; //움직임 명령을 반대로 기록해서 5개 저장하는 스택 변수
_Bool box_history[5];


void selectmap(int imap);
void record_history(char move);

_Bool check_mapfile(int n,int m) // 맵파일의 박스와 골인지점의 수를 검사하여, 수가 다르다면 오류를 출력함
{
    if(n==m)
        return 1;
    else
        return 0;
}

int getch(void) // 리눅스에서 getch() 사용을 위한 함수
{
    int ch;

    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;

    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    tcsetattr(0, TCSAFLUSH, &buf);

    ch = getchar();

    tcsetattr(0, TCSAFLUSH, &save);

    return ch;
}

void load_map(void) // 맵파일에서 데이터를 불러와 temp 에 저장하고 가공하여 map 에 저장하는 함수
{
    FILE *ifp;
    /*
     * temp: 처음 맵파일을 읽어와 데이터를 저장하는 변수
     * imap: 맵 번호
     * iy: 맵의 행(Y축) 번호
     * ix: 맵의 열(X축) 번호
     * will_load: 문자 로드 여부를 설정하는 변수
     * check_box: 맵파일의 박스의 갯수를 확인하는 변수
     * check_goals: 맵파일에 골의 위치를 확인하는 변수
     * check_error: 맵의 박스와 골의 위치를 확인하며 에러를 판별하는 변수
    */
    char temp[5][30][30] =  {'\0', };
    int imap = 0, iy = 0, ix = 0, c = 0;
    int check_box=0,check_goals=0;
    _Bool will_load = false;
    ifp = fopen("map", "r");

    while ((c = getc(ifp)) != EOF)
    {
        switch (c)
        {
            case '1':
                imap = 0;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '2':
                imap = 1;
                iy = 0;
                ix = 0;
                if(!(check_mapfile(check_box,check_goals)))
                    check_error=1;
                check_box=0;
                check_goals=0;
                will_load = false;
                break;

            case '3':
                imap = 2;
                iy = 0;
                ix = 0;
                if(!(check_mapfile(check_box,check_goals)))
                    check_error=1;
                check_box=0;
                check_goals=0;
                will_load = false;
                break;

            case '4':
                imap = 3;
                iy = 0;
                ix = 0;
                if(!(check_mapfile(check_box,check_goals)))
                    check_error=1;
                check_box=0;
                check_goals=0;
                will_load = false;
                break;

            case '5':
                imap = 4;
                iy = 0;
                ix = 0;
                if(!(check_mapfile(check_box,check_goals)))
                    check_error=1;
                check_box=0;
                check_goals=0;
                will_load = false;
                break;

            case '\n':
                //아래 if 절은 맵 파일에서 1, 2, 3, 4, 5, n을 걸러줌
                if (!will_load)
                    will_load = true;
                else
                    iy++;
                ix = 0;
                break;

            case 'e':
                if(!(check_mapfile(check_box,check_goals)))
                    check_error=1;
                check_box=0;
                check_goals=0;
                goto load_map_end;

            default:
                temp[imap][iy][ix] = c;
                ix++;
                if(c=='$')
                    check_box++;
                else if(c=='O')
                    check_goals++;
                break;
        }
    }

    load_map_end:

    fclose(ifp);
    if(check_error == 1) {
        printf("Error : Map");
        return;
    }

    for (int i1 = 0; i1 <= 4; i1++)
    {
        for (int i2 = 0; i2 <= 29; i2++)
        {
            for (int i3 = 0; i3 <= 29; i3++)
            {
                if (temp[i1][i2][i3] == '\0')
                    map[i1][i2][i3] = 'C';
                else
                    map[i1][i2][i3] = temp[i1][i2][i3];
            }
        }
    }
}

int checkXsize(int imap) // 배열의 X 사이즈를 알아내는 함수
{
    int size = 0, iy = 0;

    for (int ix = 0; ix <= 29; ix++)
    {
        if (map[imap][iy][ix] == 'C')
        {
            size = ix;
            break;
        }
    }

    return size;
}

int checkYsize(int imap, int Xsize) // 배열의 Y 사이즈를 알아내는 함수
{
    int size = 0;
    int ix = Xsize - 1;

    for (int iy = 0; iy <= 29; iy++)
    {
        if (map[imap][iy][ix] == 'C')
        {
            size = iy;
            break;
        }
    }

    return size;
}

void get_player_pos(void) // 플레이어의 위치를 찾는 함수
{
    for (int iy = 0; iy < checkYsize(current_map_no, checkXsize(current_map_no)); iy++)
    {
        for (int ix = 0; ix < checkXsize(current_map_no); ix++)
        {
            switch(nowPlayMap[iy][ix])
            {
                case '@':
                    current_player_pos[0] = ix;
                    current_player_pos[1] = iy;
            }
        }
        printf("\n");
    }
}

void check_goals(int imap) // Checking
{
    int goals_achieved = 0;
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            if ((nowPlayMap[iy][ix] == '$') && (map[imap][iy][ix] == 'O'))
                goals_achieved ++;
        }
    }

    if (goals_achieved == current_goals)
        selectmap(++current_map_no);
}

void move_player(char move, int imap) // 플레이어를 움직이는 함수
{
    if(map[imap][current_player_pos[1]][current_player_pos[0]]=='O') // 만약 플레이어가 있던 위치가 원래는 O 였으면(대문자 o)
    {
        nowPlayMap[current_player_pos[1]][current_player_pos[0]] = 'O'; // O 로 변경
    }
    else
    {
        nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '.'; // 아니면 .으로 변경
    }

    switch (move)
    {
        case 'h':// 좌
            current_player_pos[0]-=1;
            if (!is_undoing) record_history('l');
            break;
        case 'j':// 하
            current_player_pos[1]+=1;
            if (!is_undoing) record_history('k');
            break;
        case 'k':// 상
            current_player_pos[1]-=1;
            if (!is_undoing) record_history('j');
            break;
        case 'l' :// 우
            current_player_pos[0]+=1;
            if (!is_undoing) record_history('h');
            break;
    }
  
    // 골뱅이 위치를 새로 찍어주고
    nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '@';

    check_goals(imap); //플레이어가 움직일 때마다 골 여부 확인
}

void move_box(char c, int imap) // 플레이어 이동방향 앞에 박스가 존재할경우를 검사. 박스의 앞에 벽이나 또다른 박스가 있다면 움직이지 않습니다.
{
    switch (c)
    {
        case 'h':// 좌
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '$' )
            {
                nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = '$';
                if (!is_undoing) is_box_moved = 1;
                move_player(c,imap);
            }
            else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
        case 'j':// 하
            if ((nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '$' ))
            {
                nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = '$';
                if (!is_undoing) is_box_moved = 1;;
                move_player(c,imap);
            }
            else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
        case 'k':// 상
            if ((nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '$' ))
            {
                nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = '$';
                if (!is_undoing) is_box_moved = 1;
                move_player(c,imap);
            }
            else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
        case 'l' :// 우
            if ((nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '$' ))
            {
                nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = '$';
                if (!is_undoing) is_box_moved = 1;
                move_player(c,imap);
            }
            else
            {
                if (!is_undoing) is_box_moved = 0;
            }
            break;
    }

    if (!is_undoing) is_box_moved = 0;
}

void decide_move(char c, int imap) //앞에 있는 물체를 확인하고 움직임 여부를 결정하는 함수
{
    switch (c)
    {
        case 'h':// 좌
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] != '#')
            {
                if(nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] == '$')
                {
                    move_box(c, imap);
                }
                else
                {
                    move_player(c, imap);
                }
            }
            break;
        case 'j':// 하
            if (nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] != '#')
            {
                if (nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] == '$')
                {
                    move_box(c, imap);
                }
                else
                {
                    move_player(c, imap);
                }
            }
            break;
        case 'k':// 상
            if (nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] != '#')
            {
                if (nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] == '$')
                {
                    move_box(c, imap);
                }
                else
                {
                    move_player(c, imap);
                }
            }
            break;
        case 'l' :// 우
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] != '#')
            {
                if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] == '$')
                {
                    move_box(c, imap);
                }
                else
                {
                    move_player(c, imap);
                }
            }
            break;
    }
}

void printmap(int imap) // 현재 플레이하고 있는 맵을 출력
{
    system("clear");
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            printf("%c", nowPlayMap[iy][ix]);
        }
        printf("\n");
    }
}

void selectmap(int imap) // 플레이할 맵을 선택
{
    for (int i = 0; i <= 4; ++i)
        cmd_history[i] = box_history[i] = '\0';


    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            nowPlayMap[iy][ix] = map[imap][iy][ix];

            //목표 지점 카운팅
            if (nowPlayMap[iy][ix] == 'O')
                current_goals ++;
        }
    }

    get_player_pos();
}

void newgame(int imap) // 맵번호를 받아 다시 시작
{
    selectmap(imap);
    printmap(imap);
}

void record_history(char move) //플레이어의 움직임을 기록하는 함수
{
    //일반 커맨드를 입력받았을 경우
    for (int i = 0; i <= 3; ++i)
    {
        cmd_history[i] = cmd_history[i+1];
        box_history[i] = box_history[i+1];
    }

    cmd_history[4] = move;
    box_history[4] = is_box_moved;
}

void undo()
{
    is_undoing = true;
    decide_move(cmd_history[4], current_map_no);

    if (box_history[4])
    {
        switch (cmd_history[4])
        {
            case 'h':// 좌
                if(map[current_map_no][current_player_pos[1]][current_player_pos[0]+2] == 'O')
                    nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = 'O';
                else
                    nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = '.';
                nowPlayMap[current_player_pos[1]][current_player_pos[0]+1] = '$';
                break;
            case 'j':// 하
                if(map[current_map_no][current_player_pos[1]-2][current_player_pos[0]] == 'O')
                    nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = 'O';
                else
                    nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = '.';
                nowPlayMap[current_player_pos[1]-1][current_player_pos[0]] = '$';
                break;
            case 'k':// 상
                if(map[current_map_no][current_player_pos[1]+2][current_player_pos[0]] == 'O')
                    nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = 'O';
                else
                    nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = '.';
                nowPlayMap[current_player_pos[1]+1][current_player_pos[0]] = '$';
                break;
            case 'l' :// 우
                if(map[current_map_no][current_player_pos[1]][current_player_pos[0]-2] == 'O')
                    nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = 'O';
                else
                    nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = '.';
                nowPlayMap[current_player_pos[1]][current_player_pos[0]-1] = '$';
                break;
        }
    }

    printmap(current_map_no);

    for (int i = 3; i >= 0; --i)
    {
        cmd_history[i+1] = cmd_history[i];
        box_history[i+1] = box_history[i];
    }

    cmd_history[0] = '\0';
    box_history[0] = '\0';
}

void ranking(int move_count, char imap)
{
    int i,j,change,k;

    if (imap == '\n')
        printf("%d\n", move_count);
    else if (imap == '1')
        printf("%d\n", move_count);
    else if (imap == '2')
        printf("%d\n", move_count);
    else if (imap == '3')
        printf("%d\n", move_count);
    else if (imap == '4')
        printf("%d\n", move_count);
    else if (imap == '5')
        printf("%d\n", move_count);
}

void save(void){
    FILE *ofp;

    ofp = fopen("sokoban","w");
    fprintf(ofp, "%s %d", name, move_count);

    fclose(ofp);
}

void load(void){
    FILE *ifp;

    ifp = fopen("sokoban","r");
    fscanf(ifp, "%s %d", name, &move_count);
    printf("%s %d", name, move_count);

    fclose(ifp);
}

void display(void)
{
    system("clear");
    printf("h(왼쪽), j(아래), k(위), l(오른쪽)\n"
           "u(undo)\n"
           "r(replay)\n"
           "n(new)\n"
           "e(exit)\n"
           "s(save)\n"
           "f(file load)\n"
           "d(display help)\n"
           "t(top)");
}

int main(void)
{
    char command;
    int imap = 0;

    load_map();

    if (check_error == 1)
        return 0;

    printf("이름을 입력해주세요 : ");
    scanf("%10s", name);
    printf("%s", name);

    current_map_no = 0;

    selectmap(current_map_no);
    printmap(current_map_no);

    while(1)
    {
        // 맵파일 1번으로 가정, 추후 맵 선택 기능 추가 예정
        command = getch();

        switch(command)
        {
            case 'h':
            case 'j':
            case 'k':
            case 'l':
                get_player_pos();
                is_undoing = false;
                move_count++;
                decide_move(command, imap);
                printmap(current_map_no);
                break;

            case 'u':
                get_player_pos();
                undo();
                break;

            case 'r':
                newgame(current_map_no);
                break;

            case 'n':
                newgame(0);
                move_count=0;
                break;

            case 's':
                save();
                break;

            case 'f':
                load();
                break;

            case 'd':
                display();
                break;

            case 't':
                command = getch();
                ranking(move_count,command);
                break;

            case 'e':
                goto end;
                break;
        }

        printf("\n");
        // TESTING
        // i++;
        printf("commands: ");
        for (int  i = 0;  i <= 4; ++ i) {
            printf("%c", cmd_history[i]);
        }
        printf("\n");
        printf("box: ");
        for (int  i = 0;  i <= 4; ++ i) {
            printf("%d", box_history[i]);
        }
    }
    end:
    return 0;
}