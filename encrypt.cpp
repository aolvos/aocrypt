void encrypt(const char fname[], HWND hwnd)
{
    FILE *fp;
    fp = fopen(fname, "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    byte *fdata = (byte*)malloc(fsize + 1);
    fread(fdata, 1, fsize, fp);
    fclose(fp);

    char fname_key[strlen(fname) + 13] = "";
    char fname_enc[strlen(fname) + 9] = "";

    strcpy(fname_key, fname);
    strcpy(fname_enc, fname);
    strcat(fname_key, ".aocrypt.key");
    strcat(fname_enc, ".aocrypt");

    byte r[max];
    POINT p;
    int oldx = 0,oldy = 0;
    MessageBox(hwnd, "After pressing OK move your mouse around screen RANDOMLY to generate key.", "WARNING!", MB_OK | MB_ICONEXCLAMATION);
    delay(500);
    for (int i = 0; i < max; i++)
    {
        again:
        if (GetCursorPos(&p))
        {
            if ((p.x == oldx) && (p.y == oldy)) goto again;
            oldx = p.x;
            oldy = p.y;
            r[i] = (p.x*p.y) / 256;
            if (r[i] == 0) goto again;
        }
        key[i] = r[i];
        delay(10);
    }

    fp = fopen(fname_key, "wb");
    fwrite(key, 1, max, fp);
    fclose(fp);

    int ac[max];

    int f = 0;

    for (int i = 256; i<max; i++)
    {
        ac[f] = r[i] / 16;
        ac[f + 1] = r[i] - ac[f] * 16;
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
            jjj = r[ac[d + shft] + 16 * (int)(f / 16)];
            if (sec == 0) fdata[i] += r[f] + jjj;
            if (sec == 1) fdata[i] += r[f] - jjj;
            if (sec == 2) fdata[i] += r[f] * jjj;
            if (sec == 3) fdata[i] -= r[f] * jjj;
            if (sec == 4) fdata[i] -= r[f] + jjj;
            if (sec == 5) fdata[i] -= r[f] - jjj;
            if (sec == 6) fdata[i] += r[f] * r[f] + jjj;
            if (sec == 7) fdata[i] += r[f] * r[f] - jjj;
            if (sec == 8) fdata[i] += r[f] + jjj * jjj;
            if (sec == 9) fdata[i] += r[f] - jjj * jjj;
            if (sec == 10) fdata[i] += r[f] * r[f] + jjj * jjj;
            if (sec == 11) fdata[i] += r[f] * r[f] - jjj * jjj;
            if (sec == 12) fdata[i] += r[f] * jjj + r[f];
            if (sec == 13) fdata[i] += r[f] * jjj - r[f];
            if (sec == 14) fdata[i] += jjj * (r[f] + jjj);
            if (sec == 15) fdata[i] += jjj * (r[f] - jjj);
            if (fdata[i] == chk) fdata[i] += 32;
            goto repeat;
        }
        f++;
        if (f >= max / 2) f = 0, shft++;
        if (shft >= max) shft = 0;
    }

    fp = fopen(fname_enc, "wb");
    fwrite(fdata, 1, fsize, fp);
    fclose(fp);
    free(fdata);
    if(access(fname_enc, F_OK) != -1) MessageBox(hwnd, "File has been encrypted successfully.", "SUCCESS!", MB_OK | MB_ICONINFORMATION);
    else MessageBox(hwnd, "Unexpected error has been occurred!", "ERROR!", MB_OK | MB_ICONERROR);
}