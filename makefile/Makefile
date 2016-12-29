#******************************************************************************#
#* 文件名称:Makefile                                                          *#
#* 生成目标:libbalsy.so,balftp                                                *#
#* 操作系统: AIX V5.3                                                         *#
#* 数据库  : DB2 V9.5                                                         *#
#* 编译器  : vac 6(runtime library 9.0.0.2)                                   *#
#*----------------------------------------------------------------------------*#
#* 作者    ：石升辉                                                           *#
#* 创建日期：2008/07/10                                                       *#
#******************************************************************************#

#----公共变量
SHELL		=/bin/sh

#----中间件变量 
RSM		=-r Oracle_XA

PROC		=proc

#----应用路径
BINDIR		=$(HOME)/bin
LIBDIR		=$(HOME)/lib
#----编译参数
#CFLAGS		=-DDEBUG -DBAL_DEBUG $(Q64)
CFLAGS		=-DDEBUG -D__DEBUG__ -D_REENTRANT -DNOWHAT=1 -DHPUX
APPINCLUDE	=-I$(HOME)/src/incl_c -I$(HOME)/src/incl_pub -I$(DBSDIR)/incl_c -I$(TUXDIR)/include
APPLIBSDIR	=-L$(HOME) -L$(TUXDIR)/lib 
PROCINCLUDE	= include=$(DBSDIR)/incl_c include=$(HOME)/src/incl_c include=$(HOME)/src/incl_pub
COMMLIBS	=-lclntsh 
APPLIBS		=-lm -lc  $(COMMLIBS)

#----编译目标


OBJS	= gls_gl_sub.o\
gls_com_path_reg.o\
gls_account_def.o\
gls_acc_act_rel.o\
gls_acc_rel.o\
gls_acc_ys_rel.o\
gls_aces_sys_def.o\
gls_ass_mst.o\
gls_ass_mst_hst.o\
gls_auth_auth_auth.o\
gls_auth_auth_lmt.o\
gls_auth_chk_def.o\
gls_auth_code.o\
gls_auth_cond.o\
gls_auth_cond_def.o\
gls_auth_def.o\
gls_auth_flow.o\
gls_auth_inf.o\
gls_auth_item.o\
gls_auth_oper_brno.o\
gls_auth_parm.o\
gls_auth_seqn.o\
gls_auth_tel_auth.o\
gls_auth_tel_txdef.o\
gls_auth_tx_list.o\
gls_bitmap.o\
gls_br_join.o\
gls_cfg_act_dc.o\
gls_cfg_act_prop.o\
gls_cfg_prdt_prop.o\
gls_cfg_sub_rel.o\
gls_cfg_tx_intfac.o\
gls_cfg_tx_prop.o\
gls_chag_rpt_reg.o\
gls_clear_data.o\
gls_comparmholiday.o\
gls_com_branch.o\
gls_com_class.o\
gls_com_cur_no.o\
gls_com_eod_exec.o\
gls_com_holiday.o\
gls_com_item.o\
gls_com_itf_ctrl_parm.o\
gls_com_parm.o\
gls_com_sys_parm.o\
gls_com_tax_rate.o\
gls_com_tel.o\
gls_com_wd_dimen.o\
gls_com_wd_parm.o\
gls_cw_zz_reg.o\
gls_cw_hst_reg.o\
gls_cyc_chag_reg.o\
gls_data_dic.o\
gls_dc_log.o\
gls_dc_log_bk.o\
gls_dc_log_bk_1.o\
gls_dc_log_bk_2.o\
gls_dc_log_bk_3.o\
gls_dc_log_bk_4.o\
gls_dc_log_bk_5.o\
gls_dc_log_bk_6.o\
gls_dc_log_bk_7.o\
gls_dc_log_bk_8.o\
gls_dc_log_bk_9.o\
gls_dc_log_bk_10.o\
gls_dc_log_bk_11.o\
gls_dc_log_bk_12.o\
gls_dc_log_rz.o\
gls_dc_log_sy.o\
gls_dc_log_yday.o\
gls_dic_data.o\
gls_dic_name.o\
gls_doc_dic.o\
gls_ext_gl_wd.o\
gls_ext_gl_wd_cyc.o\
gls_ext_gl_wd_day.o\
gls_gl_mst.o\
gls_gl_mst_hst.o\
gls_gl_sub_bimon.o\
gls_gl_sub_day.o\
gls_gl_sub_hyear.o\
gls_gl_sub_mon.o\
gls_gl_sub_sea.o\
gls_gl_sub_ten.o\
gls_gl_sub_week.o\
gls_gl_sub_year.o\
gls_interface_def.o\
gls_item_brno_rel.o\
gls_item_clas_rel.o\
gls_item_join.o\
gls_item_prdt_rel.o\
gls_modi_gl_hst.o\
gls_modi_gl_reg.o\
gls_prdt_bal.o\
gls_prdt_bal_hst.o\
gls_prdt_def.o\
gls_reg_lst_log.o\
gls_rpt_branch.o\
gls_rpt_crt_reg.o\
gls_rpt_def.o\
gls_rpt_head.o\
gls_rpt_parm.o\
gls_rpt_table.o\
gls_rpt_tail.o\
gls_rsp_code.o\
gls_send_recv_reg.o\
gls_trace_log.o\
gls_trace_logunion.o\
gls_trace_log_bk.o\
gls_trace_log_rz.o\
gls_tx_branch_def.o\
gls_tx_br_act_def.o\
gls_tx_def.o\
gls_tx_flow_def.o\
gls_tx_log.o\
gls_tx_log_bk.o\
gls_tx_sub_def.o\
gls_tx_sub_rel.o\
gls_tx_thread.o\
gls_tyjz_reg.o\
gls_yw_refer.o\
gls_zz_gl_day.o\
gls_v_gl_item.o\
gls_v_ext_gl_item.o\
gls_v_gl_ext_item.o\
gls_v_sys_gl_item.o\
gls_v_gl_his_item.o\
gls_dc_branch.o\
gls_tx_dc_br_rel.o\
gls_com_item_p.o\
gls_com_item_tmp.o\
gls_com_branch_p.o\
gls_com_branch_h.o\
gls_account_mix_rel.o\
gls_sys_gl_wd.o\
gls_sys_gl_wd_hst.o\
gls_eod_flow_def.o\
gls_sys_param_parm_dtl.o\
gls_com_cur_rel.o\
gls_gl_sub_base.o\
gls_com_item_rel.o\
gls_gl_sub_base_bk.o\
gls_br_his.o\
gls_rpt_table_sys.o\
gls_rpt_table_ext.o\
gls_gl_sub_sy.o\
gls_gl_sub_sy_base.o\
gls_gl_sub_asy.o\
gls_zjgl_mst.o\
gls_zjgl_mst_hst.o\
gls_com_eod_exec_hst.o\
gls_ass_mst.o\
gls_ass_mst_hst.o\
gls_com_rate.o\
gls_gl_sub_ass_temp.o\
gls_ass_acc_def_bk.o\
gls_ass_acc_def.o\
gls_dc_log_ass.o\
gls_gl_sub_ass.o\
hx_dd_td_in_mst.o\
hx_ln_mst.o \
student.o

TARGET	= libpubdb.so

#----编译入口
all: $(TARGET)

libpubdb.so: $(OBJS)
	$(LD) -o $@ $(OBJS) $(APPLIBSDIR) -L$(ORACLE_HOME)/lib -lclntsh
	@cp -f $@ $(LIBDIR)


#----隐含规则
.SUFFIXES: .o .c .pc
 
.pc.o: 
	$(PROC) DYNAMIC=ANSI $(PROCINCLUDE) dbms=v7 unsafe_null=yes lines=yes iname=$*.pc
	$(CC) $(CFLAGS) $(APPINCLUDE) -c $*.c
	@rm -f $*.c
	@rm -f $*.lis
 
.c.o:
	$(CC) $(CFLAGS) $(APPINCLUDE) -c $*.c
 
 
clean: cleanup
	$(RM) $(TARGET)
cleanup:
	$(RM) $(OBJS) 
 

