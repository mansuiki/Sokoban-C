#include <stdio.h>

char temp[5][30][30]; // 처음 맵파일을 읽어와 데이터를 저장하는 변수
char map[5][30][30]; // temp 변수를 가공하여 저장하는 변수


void loadmap(void) // 맵파일에서 데이터를 불러와 temp 에 저장하고 가공하여 map 에 저장하는 함수
{
    FILE *ifp;
    int i1 = 0, i2 = 0, i3 = 0, c = 0, yes = 0;
    ifp = fopen("map", "r");

    while ((c = getc(ifp)) != EOF)
    {
        switch (c)
        {
            case '1':
                i1 = 0;
                i2 = 0;
                i3 = 0;
                yes = 0;
                break;

            case '2':
                i1 = 1;
                i2 = 0;
                i3 = 0;
                yes = 0;
                break;

            case '3':
                i1 = 2;
                i2 = 0;
                i3 = 0;
                yes = 0;
                break;

            case '4':
                i1 = 3;
                i2 = 0;
                i3 = 0;
                yes = 0;
                break;

            case '5':
                i1 = 4;
                i2 = 0;
                i3 = 0;
                yes = 0;
                break;

            case '\n':
                if (yes == 0)
                    yes = 1;
                else if (yes == 1)
                    i2++;
                i3 = 0;
                break;

            case 'e':
                goto loadmapend;

            default:
                temp[i1][i2][i3] = c;
                i3++;
                break;
        }
    }

    loadmapend:

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


int checkXsize(int i1) // 배열의 X 사이즈를 알아내는 함수
{
    int size = 0, i2 = 0;

    for (int i3 = 0; i3 <= 29; i3++)
    {
        if (map[i1][i2][i3] == 'C')
        {
            size = i3;
            break;
        }
    }

    return size;
}


int checkYsize(int i1, int Xsize) // 배열의 Y 사이즈를 알아내는 함수
{
    int size = 0;
    int i3 = Xsize - 1;

    for (int i2 = 0; i2 <= 29; i2++)
    {
        if (map[i1][i2][i3] == 'C')
        {
            size = i2;
            break;
        }
    }

    return size;
}


int main(void)
{

    loadmap();

    char map1[checkXsize(0)][checkYsize(0, checkXsize(0))];
    char map2[checkXsize(1)][checkYsize(1, checkXsize(1))];
    char map3[checkXsize(2)][checkYsize(2, checkXsize(2))];
    char map4[checkXsize(3)][checkYsize(3, checkXsize(3))];
    char map5[checkXsize(4)][checkYsize(4, checkXsize(4))];

    for (int i = 0; i <= 4; i++)
    {
        for (int i1 = 0; i1 < checkYsize(i, checkXsize(i)); i1++)
        {
            for (int i2 = 0; i2 < checkXsize(i); i2++)
            {
                switch (i)
                {
                    case 0:
                        map1[i1][i2] = map[i][i1][i2];
                        printf("%c", map1[i1][i2]);
                        break;
                    case 1:
                        map2[i1][i2] = map[i][i1][i2];
                        printf("%c", map2[i1][i2]);
                        break;
                    case 2:
                        map3[i1][i2] = map[i][i1][i2];
                        printf("%c", map3[i1][i2]);
                        break;
                    case 3:
                        map4[i1][i2] = map[i][i1][i2];
                        printf("%c", map4[i1][i2]);
                        break;
                    case 4:
                        map5[i1][i2] = map[i][i1][i2];
                        printf("%c", map5[i1][i2]);
                        break;
                }
            }
            printf("\n");
        }
        printf("\n\n");
    }



    return 0;
}
