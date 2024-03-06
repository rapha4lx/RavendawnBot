#include <Windows.h>


#include "Notification.h"


// ID da notificação
#define ID_TRAY_APP_ICON 5000

// Estrutura da notificação

NOTIFYICONDATA trayIcon;

void Notification::Initialize() {
    trayIcon.cbSize = sizeof(NOTIFYICONDATA);
    trayIcon.hWnd = NULL; // Define como NULL para indicar que não há uma janela associada
    trayIcon.uID = ID_TRAY_APP_ICON;
    trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    trayIcon.uCallbackMessage = WM_USER + 1; // Mensagem que será enviada para a janela quando o usuário interagir com o ícone
    trayIcon.hIcon = LoadIcon(NULL, IDI_INFORMATION); // Ícone para a notificação
    strcpy_s(trayIcon.szTip, "Ravendawn Bot"); // Texto da dica ao passar o mouse sobre o ícone

    // Registra o ícone na bandeja do sistema
    Shell_NotifyIcon(NIM_ADD, &trayIcon);

    // Exibe a notificação
    trayIcon.uFlags = NIF_INFO;
    strcpy_s(trayIcon.szInfo, "Success");
    strcpy_s(trayIcon.szInfoTitle, "Initialized");
    Shell_NotifyIcon(NIM_MODIFY, &trayIcon);
}

void Notification::Notification(const char* title, const char* message) {
    strcpy_s(trayIcon.szInfo, title);
    strcpy_s(trayIcon.szInfoTitle, message);
    Shell_NotifyIcon(NIM_MODIFY, &trayIcon);
}
