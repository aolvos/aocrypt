void decrypt(const char fname[], HWND hwnd)
{
    FILE *fp;
    fp = fopen(fname, "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    byte *fdata = (byte*)malloc(fsize + 1);
    fread(fdata, 1, fsize, fp);
    fclose(fp);

    char fname_key[strlen(fname) + 5] = "";
    char fname_dec[strlen(fname) + 4] = "";
    int tmp = 0;

    for (int  i = (int)strlen(fname) - 9; i >= 0; i--)
    {
        tmp++;
        if (fname[i] == '.') i = -1;
    }
    if (tmp > (int)strlen(fname) - 9) tmp = 0;
    for (int  i = 0; i < (int)strlen(fname) - 8 - tmp; i++)
    {
        fname_dec[i] = fname[i];
    }
    fname_dec[(int)strlen(fname) - 8 - tmp] = 0;

    strcat(fname_dec, "-decrypted");

    int siz = (int)strlen(fname_dec) + tmp;

    for (int  i = 0; i < tmp; i++)
    {
        fname_dec[siz - tmp + i] = fname[(int)strlen(fname) - 8 - tmp + i];
    }

    fname_dec[siz] = 0;

    strcpy(fname_key, fname);
    strcat(fname_key, ".key");

    if(access(fname_key, F_OK) != -1)
    {

        fp = fopen(fname_key, "rb");
        fread(key, 1, max, fp);
        fclose(fp);

        int ac[max];

        int f = 0;

        for (int i = 256; i<max; i++)
        {
            ac[f] = key[i] / 16;
            ac[f + 1] = key[i] - ac[f] * 16;
            f += 2;
        }

        f=0;
        int d = 0, n = 0, sec = 0, jjj = 0, chk = 0, shft = 0;

        for (long i = 0; i<fsize; i++)
        {
            n = 0;
            sec = ac[d + shft];
            repeat:
            d++;
            if (d >= max - shft) d = 0;
            if (n == 0)
            {
                n = 1;
                chk = fdata[i];
                jjj = key[ac[d + shft] + 16 * (int)(f / 16)];
                if (sec == 0) fdata[i] -= key[f] + jjj;
                if (sec == 1) fdata[i] -= key[f] - jjj;
                if (sec == 2) fdata[i] -= key[f] * jjj;
                if (sec == 3) fdata[i] += key[f] * jjj;
                if (sec == 4) fdata[i] += key[f] + jjj;
                if (sec == 5) fdata[i] += key[f] - jjj;
                if (sec == 6) fdata[i] -= key[f] * key[f] + jjj;
                if (sec == 7) fdata[i] -= key[f] * key[f] - jjj;
                if (sec == 8) fdata[i] -= key[f] + jjj * jjj;
                if (sec == 9) fdata[i] -= key[f] - jjj * jjj;
                if (sec == 10) fdata[i] -= key[f] * key[f] + jjj * jjj;
                if (sec == 11) fdata[i] -= key[f] * key[f] - jjj * jjj;
                if (sec == 12) fdata[i] -= key[f] * jjj + key[f];
                if (sec == 13) fdata[i] -= key[f] * jjj - key[f];
                if (sec == 14) fdata[i] -= jjj * (key[f] + jjj);
                if (sec == 15) fdata[i] -= jjj * (key[f] - jjj);
                if (fdata[i] == chk) fdata[i] -= 32;
                goto repeat;
            }
            f++;
            if (f >= max / 2) f = 0, shft++;
            if (shft >= max) shft = 0;
        }

        fp = fopen(fname_dec, "wb");
        fwrite(fdata, 1, fsize, fp);
        fclose(fp);
        free(fdata);
        if(access(fname_key, F_OK) != -1) MessageBox(hwnd, "File has been decrypted successfully.", "SUCCESS!", MB_OK | MB_ICONINFORMATION);
        else MessageBox(hwnd, "Unexpected error has been occurred!", "ERROR!", MB_OK | MB_ICONERROR);
    }
    else
    {
        MessageBox(hwnd, "KEY file doesn't exist!", "ERROR!", MB_OK | MB_ICONERROR);
    }
}