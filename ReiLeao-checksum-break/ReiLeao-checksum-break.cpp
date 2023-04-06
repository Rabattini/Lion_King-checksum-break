#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <tchar.h>

using namespace std;

int main() {
    // Exibe a caixa de diálogo "Abrir arquivo" para selecionar o arquivo de entrada
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = _T("");

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = _T("Arquivos Executaveis; (*.82)\0*.82\0Todos os arquivos (*.*)\0*.*\0");
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = _T("82");

    if (GetOpenFileName(&ofn)) {
        // Abre o arquivo de entrada em modo de leitura binária
        ifstream arquivo_entrada(szFile, ios::binary);
        if (arquivo_entrada.is_open()) {
            // Obtém o tamanho do arquivo
            arquivo_entrada.seekg(0, arquivo_entrada.end);
            int tamanho = arquivo_entrada.tellg();
            arquivo_entrada.seekg(0, arquivo_entrada.beg);

            // Lê todo o conteúdo do arquivo para um buffer
            char* buffer = new char[tamanho];
            arquivo_entrada.read(buffer, tamanho);
            arquivo_entrada.close();

            // Modifica o buffer nos offsets especificados
            *(int*)(buffer + 0x74C4) = 0x08005b38;
            *(int*)(buffer + 0x768A) = 0xFFD80220;

            // Cria um novo arquivo com o buffer modificado
            TCHAR caminho_arquivo[MAX_PATH] = _T("");
            _tcscpy_s(caminho_arquivo, szFile);

            size_t posicao_ponto = _tcsrchr(caminho_arquivo, '.') - caminho_arquivo;
            _tcscpy_s(caminho_arquivo + posicao_ponto, _countof(caminho_arquivo) - posicao_ponto, _T(".82_modificado"));
            ofstream arquivo_saida(caminho_arquivo, ios::binary);
            if (arquivo_saida.is_open()) {
                arquivo_saida.write(buffer, tamanho);
                arquivo_saida.close();
                cout << "Arquivo salvo com sucesso!\n";
            }
            else {
                cout << "Erro ao abrir arquivo de saída!\n";
            }

            delete[] buffer;
        }
        else {
            cout << "Erro ao abrir arquivo de entrada!\n";
        }
    }
    else {
        cout << "Operação cancelada pelo usuário!\n";
    }

  


    cout << "Rip Checksum, sigaaaaaa disney" << endl;
    getchar();
    return 0;
}
