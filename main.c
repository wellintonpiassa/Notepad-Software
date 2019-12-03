#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

// gcc main.c -lComdlg32
HWND text_box;

char text[2000];

//Função que acescenta a barra de menu na janela
void addmenus(HWND hwnd)
{
    HMENU hmenu;

    hmenu = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, 1, "Load");
    AppendMenu(hmenu, MF_POPUP, 2, "Save");
    AppendMenu(hmenu, MF_POPUP, 3, "Exit");

    SetMenu(hwnd, hmenu);
}

//Função contendo os processos do bloco de notas
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    void save_file(HWND hwnd);
    void open_file(HWND hwnd);
    DWORD text_size;
    HWND hMenu, sMenu, menuP;
    int window_width, window_height;
    switch(msg) {
        //Comandos disponiveis na janela
        case WM_COMMAND:
            switch(wParam)
            {
                case 1:
                    open_file(hwnd);
                    break;

                case 2:
                    save_file(hwnd);
                    break;

                case 3:
                    DestroyWindow(hwnd);
                    break;
            }
            break;

        case WM_CREATE:
            //Função que executa a barra de menu
            addmenus(hwnd);
            // Cria uma janela de texto, capaz de escrever em múltiplas linhas
            // e mover-se na horizontal e vertical
            text_box = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
            10, 10, 760, 530,
            hwnd, NULL, NULL, NULL);
            break;

        case WM_SIZE:
            // Recebe o comprimento e altura da janela
            window_width = LOWORD(lParam);
            window_height = HIWORD(lParam);
            // Atualiza o tamanho da caixa de texto
            MoveWindow (text_box, 0, 0, window_width, window_height,1);
            ShowWindow(text_box,SW_SHOWNORMAL);
            break;

        case WM_CLOSE:
            //Destrói a janela do bloco de notas
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

//Lê o arquivo e printa na janela do bloco de notas
void display_file(char *path)
{
    FILE *file;
    file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);
    char *data = (char *)malloc(size+1);
    fread(data, size, 1, file);
    data[size] = '\0';

    SetWindowText(text_box, data);

    fclose(file);
}

//Função que executa a abertura da janela para escolha do arquivo que deseja abrir
void open_file(HWND hwnd)
{
    OPENFILENAME ofn;

    char file_name[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All files\0*.*\0Text files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetOpenFileName(&ofn);

    display_file(ofn.lpstrFile);
}

//Função para transferir o que está na janela do bloco de notas para o arquivo
void write_file(char *path)
{
    FILE *file;
    file = fopen(path, "w");

    int size = GetWindowTextLength(text_box);
    char *data = (char *)malloc(size+1);
    GetWindowText(text_box, data, size+1);

    fwrite(data, size+1, 1, file);

    fclose(file);
}

//Função que abre a tela para escolher o diretorio que deseja salvar
void save_file(HWND hwnd)
{
    OPENFILENAME ofn;

    char file_name[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "Text File\0*.txt\0";
    ofn.nFilterIndex = 1;

    GetSaveFileName(&ofn);

    write_file(ofn.lpstrFile);
}

//Função principal (main)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char window_class_name[] = "Editor";
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.style = 0;
    wc.lpszClassName = window_class_name;
    wc.lpfnWndProc = WndProc;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    // Registra a janela com o sistema
    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Cria a janela
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        window_class_name,
        "Text Editor",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Recebe e transmite as ações do usuário
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}