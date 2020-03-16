function setup_help() {
cat <<EOF
===============*******====================
========== MUST READ HERE=================

本脚本可以一键配置新开发机中 aosp 编译环境:
请保持该脚本与下面安装的软件或者脚本放在同
一文件夹中

安装过程中，有任何问题请联系
Eswin build team
==========================================
EOF
}
function split_warning(){
	echo "############################"
	echo "###### next  step ##########"
	echo "############################"
}
function finsih_warning(){
	echo "############################"
	echo "#### INSTALL FINISHED ######"
	echo "############################"
}
function update_apt_sources(){
	if [  -f /etc/apt/sources.list ];then
	    echo  "首先备份系统自带的源"
	    sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
	fi
	echo  "使用清华源"
	if grep -q tsinghua /etc/apt/sources.list; then
	    echo "已经使用清华源"
	else
	    sudo cp tsinghua_sources.list /etc/apt/sources.list
	fi
	echo  "更新本地软件仓库"
    sudo apt update
}

function install_necessary_lib(){
	echo  "安装AOSP编译环境需要以下软件库支持:"
	echo  "git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev gcc-multilib g++-multilib "
	echo  "libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev libgl1-mesa-dev libxml2-utils "
	echo  "xsltproc unzip python adb"
	sudo apt install -y git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev libgl1-mesa-dev libxml2-utils xsltproc unzip python adb
}
function install_repo() {
	echo "正在安装 repo..."
	if [ ! -d ~/bin ]; then
		echo "创建 ~/bin 目录"
		mkdir ~/bin
	else
		echo "~/bin目录已经创建"
		echo "复制本地 repo 可执行文件到 ~/bin目录下"
	fi
	if [ -f ~/bin/repo ]; then
		echo "repo文件已经存在"
		return
	else
		echo "复制repo可执行文件到~/bin目录"
		cp repo ~/bin
		echo "repo 的执行路径添加到PATH并存入.bashrc文件中"
		echo "export PATH=~/bin:$PATH" >> ~/.bashrc
		source ~/.bashrc
	fi
}
function install_software(){
	local answer
	echo "1. 是否安装 vim? 是[y],否[n],请键入y或者n:"
	read answer
	if [ $answer == 'y' ]; then
	    sudo apt install -y vim
	else
	    echo "退出安装vim"
	fi
	split_warning

	echo "2. 是否安装 sogou拼音输入法? 是[y],否[n],请键入y或者n:"
	read answer
	#echo $answer
	if [ $answer == 'y' ]; then
	    sudo dpkg -i sogoupinyin_2.3.1.0112_amd64.deb
	    if [ $? == 0 ]; then
		echo "sogou已安装成功"
	    else
		sudo apt --fix-broken install -y
	    fi
	    echo "sogou拼音输入法安装完成"
        else
	    echo "退出安装sogou输入法"
	fi
	split_warning

	echo "3. 是否安装 sublime? 是[y],否[n],请键入y或者n:"
	read answer
	echo $answer
	if [ $answer == 'y' ]; then
	    tar -xf sublime_text_3_build_3211_x64.tar.bz2 -C ~/bin/
	    ln -s ~/bin/sublime_text_3/sublime_text ~/bin/subl
	    if [ $? == 0]; then
	        echo "sublime 已经安装成功"
	    else
		echo "sublime 可能安装失败" #不一定是由缺少库引起的
		sudo apt --fix-broken install -y
	    fi
	else
	    echo "退出安装sublime"
	fi
	split_warning

	echo "4. 是否安装 slack? 是[y],否[n],请键入y或者n:"
	read answer
	echo $answer
	if [ $answer == 'y' ]; then
	    sudo dpkg -i slack-desktop-4.2.0-amd64.deb
	    if [ $? == 0]; then
	        echo "slack安装成功"
	    else
		echo "slack 可能安装失败" #不一定是由缺少库引起的
		sudo apt --fix-broken install -y
	    fi
	else
	    echo "退出安装slack"
	fi
	split_warning

	echo "5. 是否安装 vscode? 是[y],否[n],请键入y或者n:"
	read answer
	echo $answer
	if [ $answer == 'y' ]; then
	    sudo dpkg -i slack-desktop-4.2.0-amd64.deb
	    if [ $? == 0]; then
	        echo "vscode安装成功"
	    else
		echo "vscode 可能安装失败" #不一定是由缺少库引起的
		sudo apt --fix-broken install -y
	    fi
	else
	    echo "退出安装vscode"
	fi
	finsih_warning

}
setup_help
#update_apt_sources
# install_necessary_lib
install_repo
install_software
#sudo dpkg -i google-chrome-stable_current_amd64.deb
#sudo dpkg -i code_1.43.0-1583783132_amd64.deb
#cp .vimrc ~/
