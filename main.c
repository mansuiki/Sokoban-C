#include <stdio.h>

// 캐릭터 움직이기 기본 기능 + 뼈대 작성 필요

char temp[5][30][30];


void loadmap(void)
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
                if (yes == 1)
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

}




int main(void)
{

    loadmap();
    char map[5][30][30];

    for (int i1 = 0; i1 <= 4; i1++)
    {
        for (int i2 = 0; i2 <= 29; i2++)
        {
            for (int i3 = 0; i3 <= 29; i3++)
            {
                if (temp[i1][i2][i3] == NULL)
                    map[i1][i2][i3] = ' ';
                else
                    map[i1][i2][i3] = temp[i1][i2][i3];
                printf("%c", map[i1][i2][i3]);
            }
            printf("\n");
        }

        printf("\n\n");
    }




    return 0;
}
