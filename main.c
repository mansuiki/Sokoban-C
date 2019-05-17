#include <termios.h>
#include <stdio.h>
#define true 1
#define false 0

char map[5][30][30]; // load_map 에 temp 변수를 가공하여 저장하는 변수
char nowPlayMap[30][30] = {NULL, }; // 현재 플레이하고 있는 맵을 저장하는 변수
int current_player_pos[2]; // 플레이어의 위치를 저장하는 변수
int current_goals; //목표지점의 개수

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
    */
    char temp[5][30][30] = {NULL, };
    int imap = 0, iy = 0, ix = 0, c = 0;
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
                will_load = false;
                break;

            case '3':
                imap = 2;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '4':
                imap = 3;
                iy = 0;
                ix = 0;
                will_load = false;
                break;

            case '5':
                imap = 4;
                iy = 0;
                ix = 0;
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
                goto load_map_end;

            default:
                temp[imap][iy][ix] = c;
                ix++;
                break;
        }
    }

    load_map_end:

    fclose(ifp);

    for (int i1 = 0; i1 <= 4; i1++)
    {
        for (int i2 = 0; i2 <= 29; i2++)
        {
            for (int i3 = 0; i3 <= 29; i3++)
            {
                if (temp[i1][i2][i3] == NULL)
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

void get_player_pos(int imap) // 플레이어의 위치를 찾는 함수
{
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            switch(map[imap][iy][ix])
            {
                case '@':
                    current_player_pos[0] = ix;
                    current_player_pos[1] = iy;
            }
        }
        printf("\n");
    }
}

void check_goals(int imap)
{
    int goals_achieved = 0;
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            if (nowPlayMap[iy][ix] == '$' && map[imap][iy][ix] == 'O')
                goals_achieved ++;
        }
    }

    printf("GOALS ACHIEVED: %d\nCURR GOALS: %d\n", goals_achieved, current_goals);

    if (goals_achieved == current_goals)
        printf("==YOU WIN==\n");
}

void move_player(char move,int imap) // 플레이어를 움직이는 함수
{
    check_goals(imap); //상자가 움직일 때마다 골 여부 확인

    if(map[imap][current_player_pos[1]][current_player_pos[0]]=='O') // 만약 플레이어가 있던 위치가 원래는 O 였으면(대문자 o)
    {
        nowPlayMap[current_player_pos[1]][current_player_pos[0]] = 'O'; // O 로 변경
    }
    else
    {
        nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '.'; // 아니면 .d으로 변경
    }

    switch (move)
    {
        case 'h':// 좌
            current_player_pos[0]-=1;
            break;
        case 'j':// 하
            current_player_pos[1]+=1;
            break;
        case 'k':// 상
            current_player_pos[1]-=1;
            break;
        case 'l' :// 우
            current_player_pos[0]+=1;
            break;
    }

    // 골뱅이 위치를 새로 찍어주고
    nowPlayMap[current_player_pos[1]][current_player_pos[0]] = '@';
}

void move_box(char c,int imap) // 플레이어 이동방향 앞에 박스가 존재할경우를 검사. 박스의 앞에 벽이나 또다른 박스가 있다면 움직이지 않습니다.
{
    //달러 앞을 확인
    switch (c)
    {
        case 'h':// 좌
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] != '$' )
            {
                nowPlayMap[current_player_pos[1]][current_player_pos[0]-2] = '$';
                move_player(c,imap);
            }
            break;
        case 'j':// 하
            if (nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] != '$' )
            {
                nowPlayMap[current_player_pos[1]+2][current_player_pos[0]] = '$';
                move_player(c,imap);
            }
            break;
        case 'k':// 상
            if (nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '#' && nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] != '$' )
            {
                nowPlayMap[current_player_pos[1]-2][current_player_pos[0]] = '$';
                move_player(c,imap);
            }
            break;
        case 'l' :// 우
            if (nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '#' && nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] != '$' )
            {
                nowPlayMap[current_player_pos[1]][current_player_pos[0]+2] = '$';
                move_player(c,imap);
            }
            break;
    }
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
    for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
    {
        for (int ix = 0; ix < checkXsize(imap); ix++)
        {
            printf("%c", nowPlayMap[iy][ix]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void selectmap(int imap) // 플레이할 맵을 선택
{
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

    get_player_pos(imap);
}

void newgame(void) // 첫 번쨰 맵부터 다시 시작
{
    int imap=0;

    selectmap(imap);
    printmap(imap);
}


int main(void)
{
    char command;
    int imap = 0;

    load_map();

    int i = 0;

    selectmap(imap);
    printmap(imap);

    while(1)
    {
        // 맵파일 1번으로 가정
        command = getch();
        switch(command)
        {
            case 'n':
                newgame();
                break;
        }
        decide_move(command,imap);
        printmap(imap);
        // TESTING
        // i++;
    }

    return 0;
}