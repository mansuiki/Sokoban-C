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
void ranking(char imap);

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
    {
        ranking('S');
        current_map_no++;
        selectmap(current_map_no);
    }
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

void ranking(char imap)
{
    FILE *ifp;
    ifp = fopen("ranking", "r");

    char c = 0;
    char Read[5][6][100] = {'\0',};
    int ReadRank[5][5] = {'\0',};
    char ReadName[5][5][20] = {'\0',};
    int rankint = 0, rankstart = 0, rankend = 0;
    int first = 0, i1 = 0, i2 = 0, i3 = 0;
    // first : 처음 확인 변수, i1 : 맵 지정 변수, i2 : 그 맵의 순위 지정 변수, i3 : 그 순위의 플레이어 와 움직인 횟수 변수

    while ((c = getc(ifp)) != EOF)
    {
        switch (c)
        {
            case '\n':
                if (i2 == 5)
                {
                    i2 = 0;
                    i3 = 0;
                }
                else
                {
                    i2++;
                    i3 = 0;
                }

                break;

            case '%':
                if (first == 0)
                    first = 1;
                else
                {
                    i1++;
                    i2 = 0;
                    i3 = 0;
                }
                break;

            case ':':
                rankstart = i3;
                Read[i1][i2][i3] = c;
                i3++;
                break;

            case '^':
                rankend = i3;
                // 0: 48
                // 1: 49
                // 2: 50
                // 3: 51
                // 4: 52
                // 5: 53
                // 6: 54
                // 7: 55
                // 8: 56
                // 9: 57

                int ten = rankend - rankstart - 2;
                int tempten = 1;
                for (int i = 1; i <= ten; i++)
                    tempten *= 10;
                ten = tempten;

                for (int i = rankstart + 1; i <= rankend - 1; i++) {
                    int temp = Read[i1][i2][i] - 48; // Char to int
                    rankint += temp * ten;
                    ten /= 10;
                }

                ReadRank[i1][i2 - 1] = rankint;
                rankint = 0;
                break;

            default:
                Read[i1][i2][i3] = c;
                i3++;
                break;
        }
    }
    fclose(ifp);

    for (i1 = 0; i1 <= 4; i1++)
    {
        for (i2 = 1; i2 <= 5; i2++)
        {
            for (i3 = 0; i3 <= 100; i3++)
            {
                char temp = Read[i1][i2][i3];
                if (temp == ' ')
                {
                    ReadName[i1][i2-1][i3] = ' ';
                    break;
                }
                else if (temp == ':')
                    break;
                else
                    ReadName[i1][i2-1][i3] = Read[i1][i2][i3];
            }
        }
    }




    if (imap == '\n') // t 만 입력
    {
        system("clear");
        for (i1 = 0; i1 <= 4; i1++)
        {
            printf("map%d\n", i1+1);
            for (i2 = 0; i2 <= 4; i2++)
            {
                if (ReadName[i1][i2][0] != ' ')
                    printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
            }
            printf("\n");
        }
    }
    else if (imap == '1') // t1 입력
    {
        system("clear");
        i1 = 0;
        printf("map%d\n", i1+1);
        for (i2 = 0; i2 <= 4; i2++)
        {
            if (ReadName[i1][i2][0] != ' ')
                printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
        }
        printf("\n");
    }
    else if (imap == '2') // t2 input
    {
        system("clear");
        i1 = 1;
        printf("map%d\n", i1+1);
        for (i2 = 0; i2 <= 4; i2++)
        {
            if (ReadName[i1][i2][0] != ' ')
                printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
        }
        printf("\n");
    }
    else if (imap == '3') // t3 input
    {
        system("clear");
        i1 = 2;
        printf("map%d\n", i1+1);
        for (i2 = 0; i2 <= 4; i2++)
        {
            if (ReadName[i1][i2][0] != ' ')
                printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
        }
        printf("\n");
    }
    else if (imap == '4') // t4 input
    {
        system("clear");
        i1 = 3;
        printf("map%d\n", i1+1);
        for (i2 = 0; i2 <= 4; i2++)
        {
            if (ReadName[i1][i2][0] != ' ')
                printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
        }
        printf("\n");
    }
    else if (imap == '5') // t5 input
    {
        system("clear");
        i1 = 4;
        printf("map%d\n", i1+1);
        for (i2 = 0; i2 <= 4; i2++)
        {
            if (ReadName[i1][i2][0] != ' ')
                printf("%s : %d\n", ReadName[i1][i2], ReadRank[i1][i2]);
        }
        printf("\n");
    }
    else if (imap == 'S') // Save
    {
        //name , //move_count
        //current_map_no


        for (i2 = 0; i2 <= 4; i2++)
        {
            if (ReadRank[current_map_no][i2] > move_count)
            {
                if (i2 == 4)
                {
                    ReadRank[current_map_no][i2] = move_count;
                    for (int i = 0; i <= 19; i++)
                    {
                        if (i >= 10)
                            ReadName[current_map_no][i2][i] = '\0';
                        else
                            ReadName[current_map_no][i2][i] = name[i];
                    }
                }
                else
                {
                    for (int i = 0; i <= 19; i++) // 마지막 순위의 내용을 초기화
                        ReadName[current_map_no][4][i] = '\0';
                    ReadRank[current_map_no][4] = 0;

                    for (int i = 3; i >= i2; i--) // 순위를 한개씩 내림 -> 새로 갱신될 순위까지
                    {
                        for (int a = 0; a <= 19; a++)
                            ReadName[current_map_no][i + 1][a] = ReadName[current_map_no][i][a];
                        ReadRank[current_map_no][i+1] = ReadRank[current_map_no][i];
                    }

                    ReadRank[current_map_no][i2] = move_count; // 새로운 순위 넣음
                    for (int i = 0; i <= 19; i++)
                    {
                        if (i >= 10)
                            ReadName[current_map_no][i2][i] = '\0';
                        else
                            ReadName[current_map_no][i2][i] = name[i];
                    }
                }
                break;
            }
        }


        FILE *ofp;
        ofp = fopen("ranking", "w+");

        for (i1 = 0; i1 <= 4; i1++)
        {
            fprintf(ofp, "%%map%d\n", i1+1);
            for (i2 = 0; i2 <= 4; i2++)
            {
                fprintf(ofp, "%s:%d^\n", ReadName[i1][i2], ReadRank[i1][i2]);
            }
        }
        fclose(ofp);

        move_count = 0;
    }
}

void save(void)
{
    FILE *ofp;

    ofp = fopen("sokoban","w");
    fprintf(ofp, "%s %d", name, move_count);

    fclose(ofp);
}

void load(void)
{
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
                decide_move(command, current_map_no);
                printmap(current_map_no);
                break;

            case 'u':
                get_player_pos();
                undo();
                move_count++;
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
                ranking(command);
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
        printf("\nMove_count:%d", move_count);
    }
    end:
    return 0;
}