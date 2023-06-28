#include <iostream>
#include <windows.h> // Windowsƽ̨�µ�ͷ�ļ�
#include "TestInterface.h"

typedef IPlugin* (*CreatePlugin)(); // ���庯��ָ������

int main() {
    HINSTANCE hDLL = LoadLibrary("Interface_D.dll"); // ����DLL
    if (hDLL == nullptr) {
        std::cout << "Failed to load DLL." << std::endl;
        return 1;
    }

    CreatePlugin createPluginFunc = reinterpret_cast<CreatePlugin>(GetProcAddress(hDLL, "CreatePlugin")); // ��ȡ����ָ��
    if (createPluginFunc == nullptr) {
        std::cout << "Failed to get function pointer." << std::endl;
        return 1;
    }

    IPlugin* plugin = createPluginFunc(); // ���ú��������������

    // ���������������
    std::cout << "Plugin Name: " << plugin->getName() << std::endl;
    plugin->initialize();
    plugin->execute();

    delete plugin; // ɾ���������

    FreeLibrary(hDLL); // �ͷ�DLL

    return 0;
}
