#include <stdio.h>
#include <conio.h>
#define true 1
#define false 0

char map[5][30][30]; // load_map 에 temp 변수를 가공하여 저장하는 변수
char nowPlayMap[30][30] = {NULL, };
int current_player_pos[2];


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

void get_player_pos(int imap)
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

void move_player(char c)
{
    //일단 상하좌우
    //맵파일에 골뱅이 위치를 우선 지우고
    nowPlayMap[current_player_pos[0]][current_player_pos[1]] = '.';
    switch (c)
    {
        case 'h':// 좌
            current_player_pos[0]-=1;
        case 'j':// 하
            current_player_pos[1]+=1
        case 'k':// 상
            current_player_pos[1]-=1;
        case 'l' :// 우
            current_player_pos[0]+=1;
    }
    // 골뱅이 위치를 새로 찍어주고
    nowPlayMap[current_player_pos[0]][current_player_pos[1]] = '@';
}

int main(void)
{
    char command;
    load_map();
    /*
    for (int imap = 0; imap <= 4; imap++)
    {
        get_player_pos(imap);
        printf("PLAYER_POSITION: %d, %d\n", current_player_pos[0], current_player_pos[1]);
        for (int iy = 0; iy < checkYsize(imap, checkXsize(imap)); iy++)
        {
            for (int ix = 0; ix < checkXsize(imap); ix++)
            {
                printf("%c", map[imap][iy][ix]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    */
    while(1)
    {

        // 맵파일 1번으로 가정
        command = getch();
        move_player(command);
        //
    }
    return 0;
}