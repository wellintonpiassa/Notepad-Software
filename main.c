#include <windows.h>

// gcc main.c -lComdlg32 
HWND text_box;
char text[2000];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    DWORD text_size;
    int window_width, window_height; 
    switch(msg) {
        case WM_CREATE:
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
