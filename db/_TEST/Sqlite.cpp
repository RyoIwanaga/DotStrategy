#include "../Sqlite.h"

#include <stdio.h>
#include <string>

//static int callback (void* data, int argc, char **argv, char **azColName)
//{
//	fprintf(stderr, "%s: " (const char*)data);
//	for (int i = 0; i < argc; i++) {
//		printf("%s = %s\n", 	
//	}
//}

int main(void)
{
    sqlite3* db_p = NULL;
    std::string fullpath = "test.db"; // FIXME
	int err = 0;
	sqlite3_stmt *pStmt = NULL;

	sqlite3_close(db_p);
    
	if (sqlite3_open(fullpath.c_str(), &db_p) == SQLITE_OK) {
		// �f�[�^�̒��o
		// �X�e�[�g�����g�̗p��
		err = sqlite3_prepare_v2(db_p,
				"SELECT * FROM unit where name = 'dog'", 64,
				&pStmt, NULL);

		if(err != SQLITE_OK){
			printf("NG\n");
		}
		else{
			// �f�[�^�̒��o
			while(SQLITE_ROW == (err = sqlite3_step(pStmt)) ){
				int id = sqlite3_column_int(pStmt, 0);
				const unsigned char* name = sqlite3_column_text(pStmt, 1);
				printf("id: %d, name: %s\n", id, name);
			}
//
//			if(err != SQLITE_DONE){
//				/* TODO: �G���[���� */
//			}
		}

		// �X�e�[�g�����g�̉��
		sqlite3_finalize(pStmt);
	}

	sqlite3_close(db_p);
}
