/***************************************************************************/
/* 文件名称：makedb.c                                                      */
/* 功能描述：根据配置文件table_name中的表名生成[表名].pc的DBS函数源程序    */
/* 作    者：石升辉                                                        */
/*                                                                         */
/* 创建日期：2006.07.26                                                    */
/* 说    明：PC文件存放的相对路径$DBSDIR/dbsrc                             */
/*           表名文件存放的相对路径$DBSDIR/etc/table_mame                  */
/***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"global.h"

static char *tolower(char *s);
int gen_zipblock(FILE *fpw,char *pcTabName);
int gen_debugblock(FILE *fpw,char *pcTabName,char *pcTabName2);
/****sIndex是DBS函数序号****/
static int makedb_tab(char *pcTabName,char *pcExt)
{
	FILE *fp;
	char pcFileName[128];
	char pcTabName2[64];/****首字大写的表名****/
	tolower(pcTabName);
	strcpy(pcTabName2,pcTabName);
	if(pcTabName2[0]>='a' && pcTabName2[0]<='z'){
		pcTabName2[0]=pcTabName2[0]-'a'+'A';
	}
	sprintf(pcFileName,"%s/dbsrc/%s.pc",getenv("DBSDIR"),pcTabName);
	if(pcExt[0]=='\0'){
		fp=fopen(pcFileName,"wb");
	}else{
		fp=fopen(pcFileName,"ab");
	}
	if(fp==NULL){
		exit(1);
	}	
	if(pcExt[0]=='\0'){
		fprintf(fp,"#include<stdio.h>\n");
		fprintf(fp,"#include<varargs.h>\n");
		fprintf(fp,"\n");
		fprintf(fp,"#define EXTERN\n");
		fprintf(fp,"#include\"global.h\"\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"EXEC SQL INCLUDE SQLCA;\n");
		fprintf(fp,"#include\"%s_c.h\"\n",pcTabName);
		fprintf(fp,"#define CM_SQLCODE sqlca.sqlcode==1403?100:(sqlca.sqlcode==-1?-239:sqlca.sqlcode)\n");
		if ( strcmp( pcTabName, "dd_mst" ) == 0 ||
				strcmp( pcTabName, "td_mst" ) == 0 ||
				strcmp( pcTabName, "in_mst" ) == 0 ||
				strcmp( pcTabName, "od_mst" ) == 0 ||
				strcmp( pcTabName, "mdm_ac_rel" ) == 0 ||
				strcmp( pcTabName, "ln_lo" ) == 0 ||
				strcmp( pcTabName, "ln_mst" ) == 0){
			fprintf(fp,"static char str[3000];\n");
			fprintf(fp,"static char mystery[17];\n");
		}
		fprintf(fp,"\n");
		/****外部函数声明****/
		fprintf(fp,"int %s_con ( struct %s_c , char *);\n",pcTabName, pcTabName );
		fprintf(fp,"int put_mystery( char *, char * );\n");
		/****压缩空格函数****/
		gen_zipblock(fp,pcTabName);
		gen_debugblock(fp,pcTabName,pcTabName2);
	}
	fprintf(fp,"/*****************************************************************************/\n");
	fprintf(fp,"/****                              查询函数部分[%s]                       ****/\n",pcExt);
	fprintf(fp,"/*****************************************************************************/\n");
	fprintf(fp,"int %s_Sel%s(reply,%s_c,fmtstr,va_alist)\n",pcTabName2,pcExt,pcTabName);
	fprintf(fp,"char    *reply;\n");
	fprintf(fp,"struct  %s_c *%s_c;\n",pcTabName,pcTabName);	
	fprintf(fp,"char    *fmtstr;\n");
	fprintf(fp,"va_dcl\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	int   ret;\n");
	fprintf(fp,"	char    wherelist[800];\n");
	fprintf(fp,"\n");
	fprintf(fp,"	va_list ap;\n");
	fprintf(fp,"	va_start(ap);\n");
	fprintf(fp,"	vsprintf ( wherelist , fmtstr , ap );\n");
	fprintf(fp,"	va_end(ap);\n");
	fprintf(fp,"\n");
	fprintf(fp,"	ret = %s_Dec_Sel%s( reply , \"%s\" , wherelist );\n",pcTabName2,pcExt, "%s");
	fprintf(fp,"	if (ret){\n");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		return ret;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"	ret = %s_Fet_Sel%s( %s_c , reply );\n",pcTabName2,pcExt,pcTabName);
	fprintf(fp,"	if (ret){\n");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		return ret;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"\n");
	fprintf(fp,"	%s_Clo_Sel%s();\n",pcTabName2,pcExt);
	fprintf(fp,"\n");
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	/****删除函数部分已经不用了,不再写****/
	fprintf(fp,"\n");
	fprintf(fp,"\n");
	fprintf(fp,"char *key_strcat(char *,char *);\n");
	fprintf(fp,"int %s_Dec_Sel%s ( reply , fmtstr , va_alist )\n",pcTabName2,pcExt);
	fprintf(fp,"char    *reply;\n");
	fprintf(fp,"char    *fmtstr;\n");
	fprintf(fp,"va_dcl\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	EXEC    SQL BEGIN   DECLARE SECTION;\n");
	fprintf(fp,"		char    comm[1000];\n");
	fprintf(fp,"	EXEC    SQL END     DECLARE SECTION;\n");
	fprintf(fp,"	char    wherelist[800];\n");
	fprintf(fp,"\n");
	fprintf(fp,"	va_list ap;\n");
	fprintf(fp,"	va_start(ap);\n");
	fprintf(fp,"	vsprintf ( wherelist , fmtstr , ap );\n");
	fprintf(fp,"	va_end(ap);\n");
	fprintf(fp,"\n");
	fprintf(fp,"	sprintf ( comm ,\" SELECT rowid,%s.* FROM %s WHERE  \");\n",pcTabName,pcTabName);
	fprintf(fp,"	key_strcat(comm,wherelist);\n");
	fprintf(fp,"\n");
	fprintf(fp,"	vtcp_log(\"%%s,%%d,SQLSTR=[%%s]\\n\",__FILE__,__LINE__,comm);\n");
	fprintf(fp,"	sqlca.sqlcode=0;\n");
	fprintf(fp,"	EXEC SQL PREPARE %s_sel%s FROM :comm;\n",pcTabName,pcExt);
	fprintf(fp,"	EXEC SQL DECLARE sel_%s%s CURSOR FOR %s_sel%s;\n",pcTabName,pcExt,pcTabName,pcExt);
	fprintf(fp,"\n");
	fprintf(fp,"	EXEC SQL OPEN sel_%s%s;\n",pcTabName,pcExt);
	fprintf(fp,"	if (sqlca.sqlcode){\n");
	fprintf(fp,"		sprintf(acErrMsg,\"OPEN %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		strcpy ( reply , \"D102\" );\n");
	fprintf(fp,"		return CM_SQLCODE;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"\n");
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"int %s_Fet_Sel%s ( struct %s_c *%s_c , char * reply )\n",pcTabName2,pcExt,pcTabName,pcTabName);
	fprintf(fp,"{\n");
	fprintf(fp,"\n");
	fprintf(fp,"	int   ret;\n");
	fprintf(fp,"\n");
	fprintf(fp,"	EXEC SQL FETCH sel_%s%s INTO :%s_c;\n",pcTabName,pcExt,pcTabName);
	fprintf(fp,"	if (sqlca.sqlcode&&sqlca.sqlcode!=1403){\n");
	fprintf(fp,"		sprintf(acErrMsg,\"Fetch %s error!!! %s\",sqlca.sqlcode);\n",pcTabName,"%d");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		strcpy (reply,\"D103\");\n");
	fprintf(fp,"		return CM_SQLCODE;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"	else if ( sqlca.sqlcode == 1403 ){\n");
	fprintf(fp,"		sprintf(acErrMsg,\"Fetch %s finished!!! %s\",sqlca.sqlcode);\n",pcTabName,"%d");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		strcpy (reply,\"D104\");\n");
	fprintf(fp,"		return CM_SQLCODE;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"	zip_struct(%s_c);\n",pcTabName);
	fprintf(fp,"\n");
	/*----------- 判断密押,只有od_mst需要 ------------*/
	if ( strcmp( pcTabName, "od_mst" ) == 0 ){
		fprintf(fp, "	memset( str, 0x0,sizeof(str) );\n");
		fprintf(fp, "	memset( mystery, 0x0, sizeof(mystery) );\n");

		fprintf(fp,"	ret = %s_con ( *%s_c, str );\n",pcTabName, pcTabName );
		fprintf(fp,"	ret = put_mystery( str, mystery );\n");

		fprintf(fp,"	if (strcmp(mystery,%s_c->%s )!=0){\n", pcTabName,"mac");
		fprintf(fp,"		sprintf(acErrMsg,\"%s密押错!! \" );\n",pcTabName);
		fprintf(fp,"		WRITEMSG\n");
		fprintf(fp,"		strcpy (reply,\"A035\");\n");
		fprintf(fp,"		return 1;\n");
		fprintf(fp,"	}\n");
	}
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"int %s_Clo_Sel%s (){\n",pcTabName2,pcExt);
	fprintf(fp,"	EXEC SQL CLOSE sel_%s%s;\n",pcTabName,pcExt);
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"/*****************************************************************************/\n");
	fprintf(fp,"/****                              修改函数部分                           ****/\n");
	fprintf(fp,"/*****************************************************************************/\n");
	fprintf(fp,"int %s_Dec_Upd%s( reply , fmtstr , va_alist)\n",pcTabName2,pcExt);
	fprintf(fp,"char    *reply;\n");
	fprintf(fp,"char    *fmtstr;\n");
	fprintf(fp,"va_dcl\n");
	fprintf(fp,"{\n");
	fprintf(fp,"	EXEC    SQL BEGIN   DECLARE SECTION;\n");
	fprintf(fp,"		char    comm[1000];\n");
	fprintf(fp,"	EXEC    SQL END     DECLARE SECTION;\n");
	fprintf(fp,"	char    wherelist[800];\n");
	fprintf(fp,"\n");
	fprintf(fp,"	va_list ap;\n");
	fprintf(fp,"	va_start(ap);\n");
	fprintf(fp,"	vsprintf ( wherelist , fmtstr , ap );\n");
	fprintf(fp,"	va_end(ap);\n");
	fprintf(fp,"\n");
	fprintf(fp,"	sprintf ( comm , \"SELECT rowid,%s.* FROM %s WHERE  \");\n",pcTabName,pcTabName);
	fprintf(fp,"	key_strcat(comm,wherelist);\n");
	fprintf(fp,"	strcat(comm,\" FOR UPDATE WAIT 5 \");\n");
	fprintf(fp,"	vtcp_log(\"%%s,%%d,SQLSTR=[%%s]\\n\",__FILE__,__LINE__,comm);\n");
	fprintf(fp,"	sqlca.sqlcode=0;\n");
	fprintf(fp,"	EXEC SQL PREPARE %s_upd%s FROM :comm;\n",pcTabName,pcExt);
	fprintf(fp,"\n");
	fprintf(fp,"	EXEC SQL DECLARE upd_%s%s CURSOR FOR %s_upd%s;\n",pcTabName,pcExt,pcTabName,pcExt);
	fprintf(fp,"\n");
	fprintf(fp,"	EXEC SQL OPEN upd_%s%s;\n",pcTabName,pcExt);
	fprintf(fp,"	if (sqlca.sqlcode){\n");
	fprintf(fp,"		sprintf(acErrMsg,\"open %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		strcpy ( reply , \"D102\" );\n");
	fprintf(fp,"		return CM_SQLCODE;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"\n");
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	fprintf(fp,"int %s_Fet_Upd%s( struct %s_c *%s_c , char * reply ){\n",pcTabName2,pcExt,pcTabName,pcTabName);
	fprintf(fp,"	int   ret;\n");
	fprintf(fp,"\n");
	fprintf(fp,"	EXEC SQL FETCH upd_%s%s INTO :%s_c;\n",pcTabName,pcExt,pcTabName);
	fprintf(fp,"	if ( sqlca.sqlcode && sqlca.sqlcode!=1403 ){\n");
	fprintf(fp,"		sprintf(acErrMsg,\"FETCH upd %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		strcpy ( reply , \"D103\" );\n");
	fprintf(fp,"		return CM_SQLCODE;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"	else if ( sqlca.sqlcode == 1403 ){\n");
	fprintf(fp,"		sprintf(acErrMsg,\"FETCH %s finished %s\",sqlca.sqlcode);\n",pcTabName,"%d");
	fprintf(fp,"		WRITEMSG\n");
	fprintf(fp,"		strcpy ( reply , \"D104\" );\n");
	fprintf(fp,"		return CM_SQLCODE;\n");
	fprintf(fp,"	}\n");
	fprintf(fp,"	zip_struct(%s_c);\n",pcTabName);

	/*----------- 判断密押,只有od_mst需要 ------------*/
	if ( strcmp( pcTabName, "od_mst" ) == 0 ){
		fprintf(fp, "	memset( str, 0x0,sizeof(str) );\n");
		fprintf(fp, "	memset( mystery, 0x0, sizeof(mystery) );\n");

		fprintf(fp,"	ret = %s_con ( *%s_c, str );\n",pcTabName, pcTabName );
		fprintf(fp,"	ret = put_mystery( str, mystery );\n");

		fprintf(fp,"	if(strcmp(mystery, %s_c->%s )!=0){\n",pcTabName,"mac");
		fprintf(fp,"		sprintf(acErrMsg,\"透支密押错!!! \" );\n");
		fprintf(fp,"		WRITEMSG\n");
		fprintf(fp,"		strcpy (reply,\"A035\");\n");
		fprintf(fp,"		return 1;\n");
		fprintf(fp,"	}\n");
	}
	fprintf(fp,"\n");
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	if(pcExt[0]=='\0'){
		fprintf(fp,"int %s_Upd_Upd( struct %s_c %s_c , char * reply){\n",pcTabName2,pcTabName,pcTabName);
		fprintf(fp,"	int ret;\n");
		fprintf(fp,"\n");
	
		/*----------- 生成密押 ------------*/
		if ( strcmp( pcTabName, "dd_mst" ) == 0 ||
				strcmp( pcTabName, "td_mst" ) == 0 ||
				strcmp( pcTabName, "in_mst" ) == 0 ||
				strcmp( pcTabName, "od_mst" ) == 0 ||
				strcmp( pcTabName, "mdm_ac_rel" ) == 0 ||
				strcmp( pcTabName, "ln_lo" ) == 0 ||
				strcmp( pcTabName, "ln_mst" ) == 0){
			fprintf(fp, "	memset( str, 0x0,sizeof(str) );\n");
			fprintf(fp, "	memset( mystery, 0x0, sizeof(mystery) );\n");
			fprintf(fp,"	ret = %s_con ( %s_c, str );\n",pcTabName,pcTabName );
			fprintf(fp,"	ret = put_mystery( str, mystery );\n");
	
			fprintf(fp,"	strcpy(  %s_c.%s, mystery ); \n", pcTabName,"mac");
		}
	
		fprintf(fp,"	EXEC SQL UPDATE %s SET ",pcTabName);
		gen_setblock(fp,pcTabName);
		fprintf(fp,"	WHERE rowid=:%s_c.rowid;\n",pcTabName);
	
		fprintf(fp,"	if ( sqlca.sqlcode && sqlca.sqlcode != 1403 ){\n");
		fprintf(fp,"		sprintf(acErrMsg,\"UPDATE %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
		fprintf(fp,"		WRITEMSG\n");
		fprintf(fp,"		strcpy ( reply , \"D105\" );\n");
		fprintf(fp,"		return CM_SQLCODE;\n");
		fprintf(fp,"	}\n");
	/*	fprintf(fp,"	pub_reg_rec_file(\" upd %s\"); \n",pcTabName);*/
		fprintf(fp,"\n");
		fprintf(fp,"	return 0;\n");
		fprintf(fp,"}\n");
		fprintf(fp,"\n");
		fprintf(fp,"\n");
		fprintf(fp,"int %s_Del_Upd(struct %s_c %s_c , char * reply  ){\n",pcTabName2,pcTabName,pcTabName);
		fprintf(fp,"\n");
		fprintf(fp,"	EXEC SQL DELETE FROM %s WHERE rowid=:%s_c.rowid;\n",pcTabName,pcTabName);
		fprintf(fp,"	if ( sqlca.sqlcode ){\n");
		fprintf(fp,"		sprintf(acErrMsg,\"DELETE %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
		fprintf(fp,"		WRITEMSG\n");
		fprintf(fp,"		strcpy ( reply , \"D105\" );\n");
		fprintf(fp,"		return CM_SQLCODE;\n");
		fprintf(fp,"	}\n");
		fprintf(fp,"\n");
		fprintf(fp,"	return 0;\n");
		fprintf(fp,"}\n");
		fprintf(fp,"\n");
	}
	fprintf(fp,"int %s_Clo_Upd%s( ){\n",pcTabName2,pcExt);
	fprintf(fp,"	EXEC SQL CLOSE upd_%s%s;\n",pcTabName,pcExt);
	fprintf(fp,"	return 0;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"\n");
	if(pcExt[0]=='\0'){
		fprintf(fp,"/*****************************************************************************/\n");
		fprintf(fp,"/****                              增加函数部分                           ****/\n");
		fprintf(fp,"/*****************************************************************************/\n");
		fprintf(fp,"int %s_Ins( struct %s_c %s_c , char * reply ){\n",pcTabName2,pcTabName,pcTabName);
		fprintf(fp,"	int ret;\n");
		fprintf(fp,"\n");
		/*----------- 生成密押 ------------*/
		if ( strcmp( pcTabName, "dd_mst" ) == 0 ||
				strcmp( pcTabName, "td_mst" ) == 0 ||
				strcmp( pcTabName, "in_mst" ) == 0 ||
				strcmp( pcTabName, "od_mst" ) == 0 ||
				strcmp( pcTabName, "mdm_ac_rel" ) == 0 ||
				strcmp( pcTabName, "ln_lo" ) == 0 ||
				strcmp( pcTabName, "ln_mst" ) == 0){
			fprintf(fp, "	memset( str, 0x0,sizeof(str) );\n");
			fprintf(fp, "	memset( mystery, 0x0, sizeof(mystery) );\n");
	
			fprintf(fp,"	ret = %s_con ( %s_c, str );\n", pcTabName,pcTabName );
			fprintf(fp,"	ret = put_mystery( str, mystery );\n");
	
			fprintf(fp,"	strcpy(  %s_c.%s, mystery ); \n", pcTabName,"mac");
		}
	
		fprintf(fp,"	EXEC SQL INSERT INTO %s VALUES (",pcTabName);
		gen_valblock(fp,pcTabName);
		fprintf(fp,");\n");
		fprintf(fp,"	if ( sqlca.sqlcode && sqlca.sqlcode != -1 )\n");
		fprintf(fp,"	{\n");
		fprintf(fp,"		sprintf(acErrMsg,\"INSERT %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
		fprintf(fp,"		WRITEMSG\n");
		fprintf(fp,"		strcpy ( reply , \"D107\" );\n");
		fprintf(fp,"		return CM_SQLCODE;\n");
		fprintf(fp,"	}\n");
		fprintf(fp,"	else if ( sqlca.sqlcode == -1 )\n");
		fprintf(fp,"	{\n");
		fprintf(fp,"		sprintf(acErrMsg,\"INSERT %s error %s\",sqlca.sqlcode);\n",pcTabName,"%d");
		fprintf(fp,"		WRITEMSG\n");
		fprintf(fp,"		strcpy ( reply , \"D108\" );\n");
		fprintf(fp,"		return CM_SQLCODE;\n");
		fprintf(fp,"	}\n");
	/*	fprintf(fp,"	pub_reg_rec_file(\" ins %s\"); \n",pcTabName); */
		fprintf(fp,"\n");
		fprintf(fp,"	return 0;\n");
		fprintf(fp,"}\n");
	}
	fclose(fp);
	return 0;	
}
static char *tolower(char *s){
		printf("生成[12]...\n");
	int len=strlen(s);
		printf("生成[12]...\n");
	int i;
		printf("生成[1232]...\n");
	for(i=0;i<len;i++){
		if(s[i]>='A' && s[i]<='Z'){
			s[i]=s[i]-'A'+'a';
		}
	}
	return(s);
}
static char *toupper(char *s){
	int len=strlen(s);
	int i;
	for(i=0;i<len;i++){
		if(s[i]>='a' && s[i]<='z'){
			s[i]=s[i]-'a'+'A';
		}
	}
	return(s);
}
static char *zip_space(char *s){
	int i,j;
	int len=strlen(s);
	i=j=0;
	while(s[i]!='\0'){
		if(s[i]==' ' ||s[i]=='\t' ||s[i]==0x0a ||s[i]==0x0d){
			i++;
		}else if(s[i]=='#'){
			break;
		}else{
			s[j]=s[i];
			i++;
			j++;
		}
	}
	while(j<len){
		s[j]='\0';
		j++;
	}
	return(s);
}
int gen_zipblock(FILE *fpw,char *pcTabName){
	char pcFileName[128];
	char line[1024];
	FILE *fpr;
	sprintf(pcFileName,"%s/src/incl_c/%s_c.h",getenv("HOME"),pcTabName);
	fpr=fopen(pcFileName,"rb");
	if(fpr==NULL){
		printf("无法打开文件[%s]\n",pcFileName);
		exit(1);
	}
	line[sizeof(line)-1]='\0';
	fprintf(fpw,"char *zip_tail(char *s);\n");
	fprintf(fpw,"static void zip_struct(struct %s_c* ps){\n",pcTabName);
	while(fgets(line,sizeof(line)-1,fpr)!=NULL){
		char pcFldName[32];
		zip_space(line);
		if(line[0]=='\0'){
			continue;
		}
		if(memcmp(line,"char",4)==0){
			int i;
			memset(pcFldName,'\0',sizeof(pcFldName));
			i=4;
			while(line[i]!='['){
				if(line[i]=='\0'){
					printf("非法的行在[%s_c.h]中\n[%s]\n",pcTabName,line);
					exit(1);
				}
				pcFldName[i-4]=line[i];
				i++;
			}
			fprintf(fpw,"	zip_tail(ps->%s);\n",pcFldName);
		}	
	}
	fprintf(fpw,"}\n");
	fclose(fpr);
	return(0);
}
int gen_setblock(FILE *fpw,char *pcTabName){
	char pcFileName[128];
	char line[1024];
	FILE *fpr;
	int  iFirstFlag=1;
	sprintf(pcFileName,"%s/src/incl_c/%s_c.h",getenv("HOME"),pcTabName);
	fpr=fopen(pcFileName,"rb");
	if(fpr==NULL){
		printf("无法打开文件[%s]\n",pcFileName);
		exit(1);
	}
	line[sizeof(line)-1]='\0';
	while(fgets(line,sizeof(line)-1,fpr)!=NULL){
		char pcFldName[32];
		zip_space(line);
		if(line[0]=='\0'){
			continue;
		}
		if(memcmp(line,"char",4)==0 ||
					memcmp(line,"long",4)==0 ||
					memcmp(line,"double",6)==0){
			int i;
			int keylen;
			memset(pcFldName,'\0',sizeof(pcFldName));
			if(memcmp(line,"double",6)==0){
				keylen=6;
			}else{
				keylen=4;
			}
			i=keylen;
			while(line[i]!='[' && line[i]!=';'){
				if(line[i]=='\0'){
					printf("非法的行在[%s_c.h]中\n[%s]\n",pcTabName,line);
					exit(1);
				}
				pcFldName[i-keylen]=line[i];
				i++;
			}
			if(strcmp(pcFldName,"rowid")==0){
				continue;
			}
			if(iFirstFlag){
				iFirstFlag=0;
				if(strcmp("date",pcFldName)==0||
					strcmp("group",pcFldName)==0||
					strcmp("number",pcFldName)==0) {
					fprintf(fpw,"\"%s\"=:%s_c.%s",pcFldName,pcTabName,pcFldName);
				}else{
					fprintf(fpw,"%s=:%s_c.%s",pcFldName,pcTabName,pcFldName);
				}
			}else{
				if(strcmp("date",pcFldName)==0||
					strcmp("group",pcFldName)==0||
					strcmp("number",pcFldName)==0) {
					fprintf(fpw,",\n\t\t\t\"%s\"=:%s_c.%s",pcFldName,pcTabName,pcFldName);
				}else{
					fprintf(fpw,",\n\t\t\t%s=:%s_c.%s",pcFldName,pcTabName,pcFldName);
				}
			}
		}
	}
	fprintf(fpw,"\n");
	fclose(fpr);
	return(0);
}
int gen_valblock(FILE *fpw,char *pcTabName){
	char pcFileName[128];
	char line[1024];
	FILE *fpr;
	int  iFirstFlag=1;
	sprintf(pcFileName,"%s/src/incl_c/%s_c.h",getenv("HOME"),pcTabName);
	fpr=fopen(pcFileName,"rb");
	if(fpr==NULL){
		printf("无法打开文件[%s]\n",pcFileName);
		exit(1);
	}
	line[sizeof(line)-1]='\0';
	while(fgets(line,sizeof(line)-1,fpr)!=NULL){
		char pcFldName[32];
		zip_space(line);
		if(line[0]=='\0'){
			continue;
		}
		if(memcmp(line,"char",4)==0 ||
					memcmp(line,"long",4)==0 ||
					memcmp(line,"double",6)==0){
			int i;
			int keylen;
			memset(pcFldName,'\0',sizeof(pcFldName));
			if(memcmp(line,"double",6)==0){
				keylen=6;
			}else{
				keylen=4;
			}
			i=keylen;
			while(line[i]!='[' && line[i]!=';'){
				if(line[i]=='\0'){
					printf("非法的行在[%s_c.h]中\n[%s]\n",pcTabName,line);
					exit(1);
				}
				pcFldName[i-keylen]=line[i];
				i++;
			}
			if(strcmp(pcFldName,"rowid")==0){
				continue;
			}
			if(iFirstFlag){
				iFirstFlag=0;
				fprintf(fpw,":%s_c.%s",pcTabName,pcFldName);
			}else{
				fprintf(fpw,",\n\t\t\t:%s_c.%s",pcTabName,pcFldName);
			}
		}
	}
	fclose(fpr);
	return(0);
}
int gen_debugblock(FILE *fpw,char *pcTabName,char *pcTabName2){
	char pcFileName[128];
	char line[1024];
	FILE *fpr;
	int  iFirstFlag=1;
	sprintf(pcFileName,"%s/src/incl_c/%s_c.h",getenv("HOME"),pcTabName);
	fpr=fopen(pcFileName,"rb");
	if(fpr==NULL){
		printf("无法打开文件[%s]\n",pcFileName);
		exit(1);
	}
	line[sizeof(line)-1]='\0';
	fprintf(fpw,"int %s_Debug(struct %s_c *ps){\n",pcTabName2,pcTabName);
	while(fgets(line,sizeof(line)-1,fpr)!=NULL){
		char pcFldName[32];
		zip_space(line);
		if(line[0]=='\0'){
			continue;
		}
		if(memcmp(line,"char",4)==0 ||
					memcmp(line,"long",4)==0 ||
					memcmp(line,"double",6)==0){
			int i;
			int keylen;
			memset(pcFldName,'\0',sizeof(pcFldName));
			if(memcmp(line,"double",6)==0){
				keylen=6;
			}else{
				keylen=4;
			}
			i=keylen;
			while(line[i]!='[' && line[i]!=';'){
				if(line[i]=='\0'){
					printf("非法的行在[%s_c.h]中\n[%s]\n",pcTabName,line);
					exit(1);
				}
				pcFldName[i-keylen]=line[i];
				i++;
			}
			if(memcmp(line,"char",4)==0){
				fprintf(fpw,"\tvtcp_log(\"%s_c.%s=[%%s]\\n\",ps->%s);\n",
						pcTabName,pcFldName,pcFldName);
			}else if(memcmp(line,"double",6)==0){
				fprintf(fpw,"\tvtcp_log(\"%s_c.%s=[%%f]\\n\",ps->%s);\n",
						pcTabName,pcFldName,pcFldName);
			}else if(memcmp(line,"long",4)==0){
				fprintf(fpw,"\tvtcp_log(\"%s_c.%s=[%%ld]\\n\",ps->%s);\n",
						pcTabName,pcFldName,pcFldName);
			}
		}
	}
	fprintf(fpw,"\treturn(0);\n");
	fprintf(fpw,"}\n");
	fclose(fpr);
	return(0);
}
int main(){
	char pcFileName[128];
	FILE *fp;
	char line[1024];
	sprintf(pcFileName,"%s/etc/table_name",getenv("DBSDIR"),TABNDIR);
	fp=fopen(pcFileName,"rb");
	if(fp==NULL){
		printf("无法打开文件[%s]\n",pcFileName);
		exit(1);
	}
	line[sizeof(line)-1]='\0';
	while(fgets(line,sizeof(line)-1,fp)!=NULL){
		char *pcTabName,*ptr;
		int sIndex,sCount;
		sIndex=0;
		sCount=1;
		zip_space(line);
		if(line[0]=='\0'){
			continue;
		}
		tolower(line);
		printf("生成[%s.pc]...\n",line);
		pcTabName=strtok(line,"|");
		printf("生成[%s.pc]...\n",line);
		ptr=strtok(NULL,"|");
		printf("生成[%s.pc]...\n",line);
		if(ptr==NULL){
			sCount=1;
		}else if(ptr[0]>='0' && ptr[0]<='9'){
			sCount=ptr[0]-'0';
		}
		printf("3生成[%s.pc]...\n",line);
		for(sIndex=0;sIndex<sCount;sIndex++){	
			char pcTmp[8];
			if(sIndex==0){
				memset(pcTmp,'\0',sizeof(pcTmp));
			}else{
				sprintf(pcTmp,"%d",sIndex);
			}
		printf("4生成[%s.pc]...\n",line);
			makedb_tab(pcTabName,pcTmp);	
		}
	}
	fclose(fp);
	return(0);
}
