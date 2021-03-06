#pragma once

class loginId {
private:
    int wait;
    const char filename[20] = "login.txt\0";
    const char tempfile[20] = "rough_login.txt\0";
    const char idfile[20] = "id_login.txt\0";
    FILE* stream1, * stream2;
    errno_t err;
    void Start() {
        while (this->wait) {
            cout << "waiting..." << endl;
            Sleep(50);
        }
        this->wait = 1;
    }
    void End() {
        this->wait = 0;
        Sleep(100);
    }
public:
    loginId() {
        this->wait = 0;
    }
    int Find(char name[15], char password[15]) {
        this->Start();
        int boolean = 0;
        char buff[DEFAULT_BUFLEN], id[DEFAULT_BUFLEN], thisname[DEFAULT_BUFLEN], thispassword[DEFAULT_BUFLEN];
        
        err = fopen_s(&stream1, this->filename, "r");
        if (err == 0) {
            while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
                int num = 0;
                analyse(buff, id, thisname, thispassword, &num);
                if (strcmp(thisname, name) == 0 && (password == NULL || strcmp(thispassword, password) == 0)) {
                    boolean = 1;
                    boolean = stoi(id);
                    break;
                }
            }
        }
        else cout << "Unable to open file(" << this->filename << ")" << endl;

        fclose(stream1);

        this->End();
        return boolean;
    }
    int Find(char name[15]) {
        return Find(name, NULL);
    }
    void Delete(char name[15]) {
        this->Start();
        int boolean = 0;
        char emty[2] = "\0";
        char buff[DEFAULT_BUFLEN] = "\0";
        char newbuff[DEFAULT_BUFLEN];
        int num;
        char id[DEFAULT_BUFLEN], thisname[DEFAULT_BUFLEN], thispass[DEFAULT_BUFLEN];

        //copy from login file to temporary file
        err = fopen_s(&stream2, this->tempfile, "w");
        if (err == 0)  fprintf(stream2, emty);
        fclose(stream2);

        err = fopen_s(&stream1, this->filename, "r");
        if (err != 0) { cout << "1. error while opening file\n"; goto deleteLabel; }

        err = fopen_s(&stream2, this->tempfile, "a");
        if (err != 0) { cout << "2. error while opening file\n"; goto deleteLabel; }
        while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
            num = 0;
            analyse(buff, id, thisname, thispass, &num);
            if (strcmp(name, thisname) == 0) {
                cout << "Account Deleted (" << name << ")\n";
            }
            else {
                //save in temporary file
                sprintf_s(newbuff, " %s", buff);
                fprintf(stream2, newbuff);
            }
        }
        fclose(stream2);

        fclose(stream1);

        //paste from temporary file to login file
        err = fopen_s(&stream2, this->filename, "w");
        if (err == 0)  fprintf(stream2, emty);
        fclose(stream2);

        err = fopen_s(&stream1, this->tempfile, "r");
        if (err != 0) { cout << "3. error while opening file\n"; goto deleteLabel; }

        err = fopen_s(&stream2, this->filename, "a");
        if (err != 0) { cout << "4. error while opening file\n"; goto deleteLabel; }
        while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
            //save in login file
            sprintf_s(newbuff, " %s", buff);
            fprintf(stream2, newbuff);
        }
        fclose(stream2);

        fclose(stream1);

    deleteLabel:
        this->End();
    }
    void Append(char name[15], char password[15]) {
        this->Start();
        char buff[DEFAULT_BUFLEN] = "\0";
        int num = -1;

        //read ID number
        err = fopen_s(&stream2, this->idfile, "r");     //open stream2
        if (err != 0) { cout << "1. error while opening file\n"; goto appendLabel; }
        while (fscanf_s(stream2, "%s", buff, _countof(buff)) != EOF) {
            num = stoi(buff);
        }
        fclose(stream2);             //close stream2

        num++;
        //write ID number
        err = fopen_s(&stream2, this->idfile, "w");     //open stream2
        if (err != 0) { cout << "2. error while opening file\n"; goto appendLabel; }
        sprintf_s(buff, "%d ", num);
        fprintf(stream2, buff);
        fclose(stream2);             //close stream2

        //append login details
        err = fopen_s(&stream1, this->filename, "a");   //open stream1
        if (err != 0) { cout << "3. error while opening file\n"; goto appendLabel; }
        sprintf_s(buff, " %d_%s_%s", num, name, password);
        fprintf(stream1, buff);
        fclose(stream1);             //close stream1

    appendLabel:
        this->End();
    }

    void ChangePassword(char name[15], char password[15]) {
        this->Start();

        this->End();
    }
};

class messageFile {
private:
    int wait;
    const char filename[20] = "message.txt\0";
    const char tempfile[25] = "rough_message.txt\0";
    const char idfile[20] = "id_message.txt\0";
    const char idtemp[25] = "rough_id_message.txt\0";
    FILE* stream1, * stream2;
    errno_t err;
    const char emty[2] = "\0";
    void Start() {
        while (this->wait) {
            cout << "waiting..." << endl;
            Sleep(50);
        }
        this->wait = 1;
    }
    void End() {
        this->wait = 0;
        Sleep(100);
    }

    void Paste(const char file1[25], const char file2[25]) {
        char buff[DEFAULT_BUFLEN], newbuff[DEFAULT_BUFLEN];
        err = fopen_s(&stream2, file1, "w");
        if (err != 0) {}
        fprintf(stream2, emty);
        fclose(stream2);

        err = fopen_s(&stream1, file2, "r");
        if (err != 0) {}

        err = fopen_s(&stream2, file1, "a");
        if (err != 0) {}
        while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
            sprintf_s(newbuff, " %s", buff);
            fprintf(stream2, newbuff);
        }
        fclose(stream2);

        fclose(stream1);
    }
public:
    loginId loginid;
    messageFile() {
        this->wait = 0;
    }
    
    int GetMessageNumber(char fromname[15], char toname[15], char id[25]) {
        this->Start();
        int fromidno = this->loginid.Find(fromname), toidno = this->loginid.Find(toname);
        int messageno = 0;
        int check_fromidno, check_toidno, check_messageno;
        char buff[DEFAULT_BUFLEN];
        char newbuff[50];
        char word[3][30];

        // Copy
        err = fopen_s(&stream2, this->idtemp, "w");
        if (err != 0) {}
        fprintf(stream2, emty);
        fclose(stream2);

        err = fopen_s(&stream1, this->idfile, "r");
        if (err != 0) {}

        err = fopen_s(&stream2, this->idtemp, "a");
        if (err != 0) {}
        while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
            int num;
            split(word, buff, &num);
            check_fromidno = stoi(word[0]);
            check_toidno = stoi(word[1]);
            check_messageno = stoi(word[2]);
            if (check_fromidno == fromidno && check_toidno == toidno) {
                check_messageno++;
                messageno = check_messageno;
            }
            sprintf_s(newbuff, " %d,%d,%d", check_fromidno, check_toidno, check_messageno);
            fprintf(stream2, newbuff);
        }

        fclose(stream1);
        
        // if messageno = 0 (This must be their first chat)
        if (!messageno) {
            messageno++;
            sprintf_s(newbuff, " %d,%d,%d", fromidno, toidno, messageno);
            fprintf(stream2, newbuff);
        }
        fclose(stream2);

        // Paste
        Paste(this->idfile, this->idtemp);

        char thisid[50];
        sprintf_s(thisid, "%d,%d,%d", fromidno, toidno, messageno);
        stringcpy(id, thisid);
        
        this->End();
        return messageno;
    }
    
    // num = 0 -> sent, not delivered            -> FROM online TO offline
    // num = 1 -> sent, delivered, acknowledged  -> FROM online TO online
    char* Write(char fromname[15], char toname[15], char* message, char sent[25], char receive[25], char read[25]) {
        char sentence[DEFAULT_BUFLEN * 5];
        char id[25];
        int receive_to_update = 0, receive_from_update = 0, read_from_update = 0,
            delete_to_update = 0, emoji_from_update = 0, emoji_to_update = 0;
        char deleted[] = "false";
        char emoji[] = "false";
        char thisreceive[50], thisread[50], thissend[50];
        char thismessage[DEFAULT_BUFLEN];

        GetMessageNumber(fromname, toname, id);

        replaceChar(thissend, sent, ' ', '*');
        //replaceChar(thissend, '\n', '\0');
        replaceChar(thismessage, message, ' ', '*');

        if (receive != NULL || read != NULL) {
            receive_to_update = 0; // delivered
            read_from_update = 0; // delivered
            receive_from_update = 0; // acknowledged
            replaceChar(thisreceive, receive, ' ', '*');
            //replaceChar(thisreceive, '\n', '\0');
            replaceChar(thisread, read, ' ', '*');
            //replaceChar(thisread, '\n', '\0');
        }
        else {
            receive_to_update = 1; // not delivered
            read_from_update = 0; // not delivered
            receive_from_update = 0; // not acknowledged
            sprintf_s(thisreceive, "false\0");
            sprintf_s(thisread, "false\0");
        }

        sprintf_s(sentence, " %s_%s_%s_%s_%s_%s_%d_%d_%s_%d_%s_%d_%s_%d_%d", 
            fromname, toname, id, thismessage, thissend, thisreceive,
            receive_to_update, receive_from_update, thisread, read_from_update,
            deleted, delete_to_update, emoji, emoji_from_update, emoji_to_update);

        cout << "Stored in file: " << sentence << endl;

        this->Start();
        err = fopen_s(&stream1, this->filename, "a");
        if(err!=0){}
        fprintf(stream1, sentence);
        fclose(stream1);
        this->End();

        return id;
    }
    
    // fetch messages when back to online
    void Fetch(SOCKET localClientSocket, char name[15]) {
        this->Start();
        char buff[DEFAULT_BUFLEN];
        char newbuff[DEFAULT_BUFLEN];
        char word[15][DEFAULT_BUFLEN];

        // Variables
        char id[25];
        int receive_to_update, receive_from_update, read_from_update,
            delete_to_update, emoji_from_update, emoji_to_update;
        char deleted[10];
        char emoji[DEFAULT_BUFLEN];
        char thisreceive[50], thisread[50], thissend[50];
        char thismessage[DEFAULT_BUFLEN];
        char fromname[15], toname[15];

        char thisthismessage[DEFAULT_BUFLEN], thisthisreceive[50], thisthisread[50], thisthissend[50];
        char newmessage[DEFAULT_BUFLEN], note[50] = "\0";

        // Copy
        err = fopen_s(&stream2, this->tempfile, "w");
        if (err != 0) {}
        fprintf(stream2, emty);
        fclose(stream2);

        err = fopen_s(&stream1, this->filename, "r");
        if (err != 0) {}

        err = fopen_s(&stream2, this->tempfile, "a");
        if (err != 0) {}
        while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
            split(word, buff);
            strcpy_s(fromname, word[0]); strcpy_s(toname, word[1]); strcpy_s(id, word[2]);
            strcpy_s(thismessage, word[3]); strcpy_s(thissend, word[4]); strcpy_s(thisreceive, word[5]);
            receive_to_update = stoi(word[6]); receive_from_update = stoi(word[7]);
            strcpy_s(thisread, word[8]); read_from_update = stoi(word[9]);
            strcpy_s(deleted, word[10]); delete_to_update = stoi(word[11]);
            strcpy_s(emoji, word[12]); emoji_from_update = stoi(word[13]); emoji_to_update = stoi(word[14]);
            int boolean = 0;
            if (strcmp(name, fromname) == 0 && strcmp(deleted, "false") == 0) {
                if (receive_from_update) {
                    receive_from_update = 0;
                    boolean = 1;
                    sprintf_s(note, "message delivered, ");
                }
                if (read_from_update) {
                    read_from_update = 0;
                    boolean = 1;
                    if (strcmp(thisread, "false") == 0) {
                        sprintf_s(note, "%smarked as unread, ", note);
                    }
                    else {
                        sprintf_s(note, "%smessage seen, ", note);
                    }
                }
                if (emoji_from_update) {
                    emoji_from_update = 0;
                    boolean = 1;
                    sprintf_s(note, "%semoji changed, ", note);
                }
            }
            else if (strcmp(name, toname) == 0 && strcmp(deleted, "false") == 0) {
                if (receive_to_update) {
                    receive_to_update = 0;
                    boolean = 3;
                    sprintf_s(note, "new message, ");
                    gettime(thisreceive);
                    gettime(thisread);
                    replaceChar(thisreceive, ' ', '*');
                    replaceChar(thisread, ' ', '*');
                }
                if (emoji_to_update) {
                    emoji_to_update = 0;
                    if (boolean == 3)
                        boolean = 3;
                    else
                        boolean = 4;
                    sprintf_s(note, "%semoji changed, ", note);
                }
            }
            else if (strcmp(name, toname) == 0 && delete_to_update) {
                delete_to_update = 0;
                boolean = 2;
                strcpy_s(deleted, "true");
                sprintf_s(note, "message deleted, ");
            }
            if (boolean) {
                Sleep(300);
                note[strlen(note) - 2] = '\0';
                replaceChar(thisthismessage, thismessage, '*', ' ');
                replaceChar(thisthissend, thissend, '*', ' ');
                replaceChar(thisthisreceive, thisreceive, '*', ' ');
                replaceChar(thisthisread, thisread, '*', ' ');
                if (boolean == 1) {
                    formatFromMessage(newmessage, thisthismessage, id, toname, thisthissend, thisthisreceive, thisthisread, emoji, note);
                    sendd(localClientSocket, newmessage, name);
                }
                else if (boolean == 2 || boolean == 3 || boolean == 4) {
                    if (boolean == 2) {
                        strcpy_s(thisthismessage, "******");
                    }
                    formatToMessage(newmessage, thisthismessage, id, fromname, emoji, note);
                    sendd(localClientSocket, newmessage, name);
                    if (boolean == 3) {
                        sprintf_s(note, "message delivered, message seen");
                        formatFromMessage(newmessage, thisthismessage, id, toname, thisthissend, thisthisreceive, thisthisread, emoji, note);
                        SOCKET valueClientSocket = client->Find(fromname);
                        if (valueClientSocket != INVALID_SOCKET) {
                            sendd(valueClientSocket, newmessage, fromname);
                            receive_from_update = 0; 
                            read_from_update = 0;
                        }
                        else {
                            receive_from_update = 1; 
                            read_from_update = 1;
                        }
                    }
                }
                sprintf_s(newbuff, " %s_%s_%s_%s_%s_%s_%d_%d_%s_%d_%s_%d_%s_%d_%d",
                    fromname, toname, id, thismessage, thissend, thisreceive,
                    receive_to_update, receive_from_update, thisread, read_from_update,
                    deleted, delete_to_update, emoji, emoji_from_update, emoji_to_update);
            }
            else {
                sprintf_s(newbuff, " %s", buff);
            }
            fprintf(stream2, newbuff);
        }
        fclose(stream2);

        fclose(stream1);

        // Paste
        Paste(this->filename, this->tempfile);

        this->End();
    }

    // number = 0 -> delete
    // number = 1 -> read
    // number = 2 -> unread
    // number = 3,4 -> from/to emoji
    int Edit(char id[25], char name[15], int number = 0, char letter[DEFAULT_BUFLEN] = NULL) {
        // number = 0       -> name -> fromname
        // number = 1,2     -> name -> toname,            letter -> date time month
        // number = 3,4       -> name -> from/to name,    letter -> emoji
        // number = 5,6
        char word[3][30];
        char sentence[25] = "\0";
        int boolean = 0;
        int fromnum, tonum, namenum;
        split(word, id);
        fromnum = stoi(word[0]);
        tonum = stoi(word[1]);
        namenum = loginid.Find(name);
        if (number == 3 && namenum == tonum) {
            number = 4;
        }
        if (number == 5 && namenum == tonum) {
            number = 6;
        }
        if ((number == 0 && namenum == fromnum)
            || ((number == 1 || number == 2) && namenum == tonum)
            || (number == 3 && namenum == fromnum)
            || number == 4 
            || (number == 5 && namenum == fromnum)
            || number == 6) {
            this->Start();
            char buff[DEFAULT_BUFLEN], newbuff[DEFAULT_BUFLEN];
            char word[15][DEFAULT_BUFLEN];

            // Copy
            err = fopen_s(&stream2, this->tempfile, "w");
            if (err != 0) {}
            fprintf(stream2, emty);
            fclose(stream2);

            err = fopen_s(&stream1, this->filename, "r");
            if (err != 0) {}

            err = fopen_s(&stream2, this->tempfile, "a");
            if (err != 0) {}
            while (fscanf_s(stream1, "%s", buff, _countof(buff)) != EOF) {
                split(word, buff);
                if (strcmp(word[2], id) == 0 && strcmp(word[10], "false") == 0) {
                    // number = 0 -> delete
                    if (number == 0 && strcmp(word[10], "true") != 0) {
                        boolean = 1;
                        char deleted[] = "true";
                        int delete_to_update;
                        char toname[15];
                        strcpy_s(toname, word[1]);
                        SOCKET valueClientSocket = client->Find(toname);
                        if (valueClientSocket != INVALID_SOCKET) {
                            delete_to_update = 0;
                            char newmessage[DEFAULT_BUFLEN];
                            char emoji[DEFAULT_BUFLEN];
                            char note[] = "message deleted";
                            char message[] = "******";
                            strcpy_s(emoji, word[12]);
                            formatToMessage(newmessage, message, id, name, emoji, note);
                            sendd(valueClientSocket, newmessage, toname);
                        }
                        else
                            delete_to_update = 1;
                        sprintf_s(newbuff, " %s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%d_%s_%s_%s",
                            word[0], word[1], word[2], word[3], word[4], word[5],
                            word[6], word[7], word[8], word[9], deleted,
                            delete_to_update, word[12], word[13], word[14]);
                    }
                    // number = 1 -> read
                    // number = 2 -> unread
                    else if ((number == 1 && strcmp(word[8], "false") == 0) ||
                        (number == 2 && strcmp(word[8], "false") != 0)) {
                        boolean = number + 1;
                        char read[50];
                        int read_from_update;
                        if (number == 1)
                            strcpy_s(read, letter);
                        else if (number == 2)
                            strcpy_s(read, "false");
                        char fromname[15];
                        strcpy_s(fromname, word[0]);
                        SOCKET valueClientSocket = client->Find(fromname);
                        if (valueClientSocket != INVALID_SOCKET) {
                            read_from_update = 0;
                            char newmessage[DEFAULT_BUFLEN];
                            char note[50];
                            if (number == 1)
                                strcpy_s(note, "message seen");
                            else if (number == 2)
                                strcpy_s(note, "marked as unread");
                            formatFromMessage(newmessage, word[3], word[2], word[1], word[4], word[5], read, word[12], note);
                            sendd(valueClientSocket, newmessage, word[0]);
                            replaceChar(read, ' ', '*');
                        }
                        else
                            read_from_update = 1;
                        sprintf_s(newbuff, " %s_%s_%s_%s_%s_%s_%s_%s_%s_%d_%s_%s_%s_%s_%s",
                            word[0], word[1], word[2], word[3], word[4], word[5],
                            word[6], word[7], read, read_from_update, word[10],
                            word[11], word[12], word[13], word[14]);
                    }
                    // number = 3 -> from emoji
                    // number = 4 -> to emoji
                    else if ((number == 3 || number == 4) || ((number == 5 || number == 6) && strcmp(word[12], "false") != 0)) {
                        boolean = number + 1;
                        char emoji[50];
                        strcpy_s(emoji, word[12]);
                        char newemoji[50]="\0";
                        int i;
                        int emoji_from_update = 0, emoji_to_update = 0;
                        if (strcmp(emoji, "false") == 0) {
                            if (number == 3) {
                                sprintf_s(newemoji, "%s,0,0,0", letter);
                            }
                            else if (number == 4) {
                                sprintf_s(newemoji, "0,0,0,%s", letter);
                            }
                        }
                        else {
                            char thisletter[emoji_len][30];
                            split(thisletter, letter);
                            char thisemoji[emoji_len * 2][30];
                            split(thisemoji, emoji);
                            if (number == 3 || number == 4) {
                                for (i = 0; i < emoji_len * 2; i++) {
                                    //cout << number << "// " << i - emoji_len << ") " << "thisemoji " << thisemoji[i] << " thisletter " << thisletter[i - emoji_len] << endl;
                                    if ((strcmp(thisemoji[i], "1") == 0) ||
                                        (number == 3 && (i < emoji_len && strcmp(thisletter[i], "1") == 0)) ||
                                        (number == 4 && (i >= emoji_len && strcmp(thisletter[i - emoji_len], "1") == 0))
                                        )
                                        sprintf_s(newemoji, "%s1,", newemoji);
                                    else
                                        sprintf_s(newemoji, "%s0,", newemoji);
                                }
                            }
                            else if (number == 5 || number == 6) {
                                for (i = 0; i < emoji_len * 2; i++) {
                                    //cout << number << "// " << i - emoji_len << ") " << "thisemoji " << thisemoji[i] << " thisletter " << thisletter[i - emoji_len] << endl;
                                    if ((strcmp(thisemoji[i], "0") == 0) ||
                                        (number == 5 && (i < emoji_len && strcmp(thisletter[i], "1") == 0)) ||
                                        (number == 6 && (i >= emoji_len && strcmp(thisletter[i - emoji_len], "1") == 0))
                                        )
                                        sprintf_s(newemoji, "%s0,", newemoji);
                                    else
                                        sprintf_s(newemoji, "%s1,", newemoji);
                                }
                            }
                            newemoji[strlen(newemoji) - 1] = '\0';
                            //cout << "Final: " << newemoji << endl;
                        }
                        if (number == 3 || number == 5) { //from
                            char fromname[15], toname[15];
                            strcpy_s(fromname, word[0]);
                            strcpy_s(toname, word[1]);
                            SOCKET valueClientSocket = client->Find(toname);
                            if (valueClientSocket != INVALID_SOCKET) {
                                emoji_to_update = 0;
                                char newmessage[DEFAULT_BUFLEN];
                                char note[50];
                                strcpy_s(note, "emoji changed");
                                formatToMessage(newmessage, word[3], word[2], fromname, newemoji, note);
                                sendd(valueClientSocket, newmessage, word[0]);
                            }
                            else
                                emoji_to_update = 1;
                        }
                        else if (number == 4 || number == 6) { //to
                            char fromname[15], toname[15];
                            strcpy_s(fromname, word[0]);
                            strcpy_s(toname, word[1]);
                            SOCKET valueClientSocket = client->Find(fromname);
                            if (valueClientSocket != INVALID_SOCKET) {
                                emoji_from_update = 0;
                                char newmessage[DEFAULT_BUFLEN];
                                char note[50];
                                strcpy_s(note, "emoji changed"); 
                                formatFromMessage(newmessage, word[3], word[2], toname, word[4], word[5], word[8], newemoji, note);
                                sendd(valueClientSocket, newmessage, word[0]);
                            }
                            else
                                emoji_from_update = 1;
                        }
                        sprintf_s(newbuff, " %s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%d_%d",
                            word[0], word[1], word[2], word[3], word[4], word[5],
                            word[6], word[7], word[8], word[9], word[10],
                            word[11], newemoji, emoji_from_update, emoji_to_update);
                    }

                    else {
                        sprintf_s(newbuff, " %s", buff);
                    }
                }
                else {
                    sprintf_s(newbuff, " %s", buff);
                }
                fprintf(stream2, newbuff);
            }
            fclose(stream2);

            fclose(stream1);

            // Paste
            Paste(this->filename, this->tempfile);

            this->End();
        }
        return boolean;
    }
};

class Myfiles {
public:
    messageFile messagefile;
};