import os
import shutil
import time
import zipfile
import re
import json
import urllib
import urllib.request
from optparse import OptionParser
from http import client
import hashlib
import uuid
import sys
import logging
from os.path import getsize
from xml.etree import ElementTree

#脚本所在目录
script_dir = os.path.split(os.path.realpath(__file__))[0]+"\\"

#基准目录，也就是源码根目录
base_dir = os.path.abspath(script_dir+"..\\..")+"\\"

#解析命令行参数
parser = OptionParser()
parser.add_option("-r", action="store_true", help="release stable version")
parser.add_option("--nu", action="store_true", help="don't update source code")
parser.add_option("--nrb", action="store_true", help="not rebuild project")
parser.add_option("--revert", action="store_true", help="revert all change")
(options, args) = parser.parse_args()

def add_git_to_path() :
    """git程序加到path环境变量"""
    drives = ("C:", "D:", "E:", "F:")
    for drive in drives :
        if os.path.exists(drive+"\\Program Files (x86)\\Git\\bin\\git.exe") :
            os.environ["PATH"]=os.environ["PATH"]+";"+drive+"\\Program Files (x86)\\Git\\bin"
            break;
        if os.path.exists(drive+"\\Program Files\\Git\\bin\\git.exe") :
            os.environ["PATH"]=os.environ["PATH"]+";"+drive+"\\Program Files\\Git\\bin"
            break;
     
    #不加此行环境变量，执行git log会警告，需手动按回车键
    os.environ["TERM"]  = "xterm"
    
def add_7z_to_path() :
    """7z程序加到path环境变量"""
    drives = ("C:", "D:", "E:", "F:")
    for drive in drives :
        if os.path.exists(drive+"\\Program Files (x86)\\7-Zip\\7z.exe") :
            os.environ["PATH"]=os.environ["PATH"]+";"+drive+"\\Program Files (x86)\\7-Zip"
            break;
        if os.path.exists(drive+"\\Program Files\\7-Zip\\7z.exe") :
            os.environ["PATH"]=os.environ["PATH"]+";"+drive+"\\Program Files\\7-Zip"
            break;
            
def get_git_commit(path) :
    """获取指定目录下的git提交串"""
    git_commit = "0000000"
    if not os.path.exists(path) :
        return git_commit
    
    os.chdir(path)
    pipe = os.popen("git log -1 --format=%h")
    os.chdir(script_dir)
    git_commit = pipe.read()
    return git_commit.strip('\n')
            
def get_version_string() :
    """
    获取版本号字符串，版本号如：ver1.9.0.100.a687add_758 说明ver1.9.0.[索引号].[git_commit]_[eci svn]，
    若正式版会在索引号号前加字母r，索引号是本地维护的编译代号逐渐递增，保存在脚本所在目录的build_code.txt文件
    """
    build_code = 1
    if os.path.exists(script_dir+"build_code.txt") : 
        file_handle = open(script_dir+"build_code.txt", "r")        
        build_code = int(file_handle.read())
        file_handle.close()
    build_code_file_handle = open(script_dir+"build_code.txt", "w")
    build_code_file_handle.seek(0)
    build_code_file_handle.write(str(build_code+1))
    build_code_file_handle.close()
    
    version = ""
    if options.r :  #正式版
        version = "ver2.0.r%d.%s" % (build_code, get_git_commit(base_dir ))
    else :
        version = "ver2.0.%d.%s" % (build_code, get_git_commit(base_dir ))
    return version

def git_update(path) :
    """更新源码"""
    if options.nu :
        return
        
    git_update_cmd = "git pull"
    os.chdir(path)
    if os.system(git_update_cmd) != 0 :
        raise Exception("failed to update source code")
    os.chdir(script_dir)         

def get_program_path(path_suffix, program_name) :
    """
    @name 获取可执行程序所在路径
    @param path_suffix路径后缀不带盘符如"Program Files (x86)\7-Zip"
    @param program_name 程序名称如7z.exe
    @return 可执行程序完整路径，如果未找到返回program_name
    """
    disk_drives = []
    disk_drives.append("C:\\");
    disk_drives.append("D:\\");
    disk_drives.append("E:\\");
    disk_drives.append("F:\\");
    
    for disk_drive in disk_drives :
        program_path = disk_drive+path_suffix+"\\"
        if os.path.exists(program_path+program_name) :
            return program_path
    
    return program_name
    
def compile_icr() :
    """编译工程"""
    devenv_path = get_program_path("Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\IDE\\", "devenv.exe")
    if devenv_path != "" :
        os.environ["PATH"]=os.environ["PATH"]+";"+devenv_path
        
    compile_cmd = "devenv driverdll.sln"
    if options.nrb :
        compile_cmd += " /Build Release"
    else :
        compile_cmd += " /Rebuild Release"
        
    os.chdir(base_dir )
    if os.system(compile_cmd) == 0 :
        print("success to compile project")
        os.chdir(script_dir)
    else :
        raise Exception("failed to compile project")
        
def revert_file(file) :
    """回滚版本文件"""    
    git_revert_cmd = "git checkout \"%s\"" % file
    os.system(git_revert_cmd)
    
def get_bin_file_list() :
    """获取bin文件列表，文件路径为绝对路径"""
    src_bin_dir = script_dir+"..\\Drivers\\lianzhi\\"
    bin_file_list = []
    bin_file_list.append(src_bin_dir+"BERR.dll")
    bin_file_list.append(src_bin_dir+"COMP.dll")
    bin_file_list.append(src_bin_dir+"CRDR.dll")
    bin_file_list.append(src_bin_dir+"ICCR.dll")
    bin_file_list.append(src_bin_dir+"PRNT.dll")
    bin_file_list.append(src_bin_dir+"PRNTEXE.exe")
    bin_file_list.append(src_bin_dir+"VOIC.dll")
    bin_file_list.append(src_bin_dir+"ScreenUtil.dll")
    bin_file_list.append(src_bin_dir+"ABCDll.dll")
    bin_file_list.append(src_bin_dir+"ABCDllv2.dll")
    bin_file_list.append(src_bin_dir+"TxPrnMod.dll")
    bin_file_list.append(script_dir+"..\\..\\doc\\使用帮助文档.docx")
    return bin_file_list

def get_ext_files_from_dir(dir, ext=None) :
    """遍历指定目录，获取指定扩展名的文件列表"""
    all_files = []
    need_ext_filter = (ext != None)
    for root, dirs, files in os.walk(dir) :
        for file in files :
            file_path = os.path.join(root, file)
            extension = os.path.splitext(file_path)[1][1:]
            if not need_ext_filter :
                all_files.append(file_path)
            elif extension == ext :
                all_files.append(file_path)
    return all_files   

def copy_folder(source_dir, target_dir):
    """递归拷贝文件夹"""
    for item in os.listdir(source_dir):
        source_item = os.path.abspath(os.path.join(source_dir, item))
        target_item = os.path.abspath(os.path.join(target_dir, item))
        if os.path.isfile(source_item):   
            if not os.path.exists(target_dir):
                os.makedirs(target_dir)
            shutil.copy(source_item, target_item)
        elif os.path.isdir(source_item):
            copy_folder(source_item, target_item)
            
def copy_release_file(release_dir) :
    #拷贝bin文件
    release_bin_dir = release_dir+"driverdll\\Drivers\\lianzhi\\"
    os.makedirs(release_bin_dir)
    for file in get_bin_file_list() :
        shutil.copy(file, release_bin_dir)
    
    #拷贝conf目录
    src_bin_dir = script_dir+"..\\Drivers\\lianzhi\\"
    copy_folder(src_bin_dir+"config\\", release_bin_dir+"config\\")
    copy_folder(base_dir+"config\\", release_bin_dir+"config\\")
    copy_folder(base_dir+"resource\\", release_bin_dir+"resource\\")
    
def create_version_file(version_file, version) :    
    """创建版本号文件"""
    version_file = open(version_file, 'w')
    version_file.write(version)
    version_file.close()
        
def compress_full_release_file(release_path, zipfile_path) :
    """压缩完整发布包"""
    filelist = []
    for root, dirs, files in os.walk(release_path):
        for name in files:
            filelist.append(os.path.join(root, name))
     
    zf = zipfile.ZipFile(zipfile_path, "w", zipfile.zlib.DEFLATED)
    for tar in filelist:
        arcname = tar[len(release_path):]
        zf.write(tar,arcname)
    zf.close()
    
def revert_all_change() :
    """回滚所有改变"""
    if options.revert :
        git_revert_cmd = "git checkout \"%s\"" % base_dir .strip('\\')
        os.system(git_revert_cmd) 
    
def copy_pdb_file(src_bin_dir, pdb_path) :
    if os.path.exists(pdb_path) :
        shutil.rmtree(pdb_path)
    os.makedirs(pdb_path)    

    pdb_file_list = get_ext_files_from_dir(src_bin_dir, "pdb")
    for file_path in pdb_file_list :
        shutil.copy(file_path, pdb_path)

def compress_pdb_file(pdb_path, compress_file) :
    if os.path.exists(compress_file) :
        os.remove(compress_file)
        
    filelist = []
    for root, dirs, files in os.walk(pdb_path):
        for name in files:
            filelist.append(os.path.join(root, name))     

    zf = zipfile.ZipFile(compress_file, "w", zipfile.zlib.DEFLATED)
    for tar in filelist:
        arcname = tar[len(pdb_path):]
        zf.write(tar,arcname)
    zf.close()
    
def save_setting(setting_file, settings) :
    """
    @name 保存设置
    @param setting_file 设置文件
    @param settings 设置内容 dict对象
    """
    content = json.dumps(settings, sort_keys=True, indent=4, ensure_ascii=False)
    file_handle = open(setting_file, "w", 1, "utf-8")
    file_handle.write(content)
    file_handle.close()
    
def modify_conf(conf_file, key, value) :
    """修改配置，配置文件需要ini格式"""
    file_handle = open(conf_file, "r")
    lines = file_handle.readlines()
    file_handle.close()
    
    already_modify = False
    file_handle = open(conf_file, "w")
    for line in lines :
        if line.find(key+"=")  == 0:
            file_handle.write(key+"="+value)
            already_modify = True
        else :
            file_handle.write(line)
    file_handle.close()
    
    if not already_modify :
        raise Exception("cannot modify conf, key is "+key)
        
def replace_app_version(app_version_file, version) :    
    """替换版本号"""    
    file_handle = open(app_version_file, "r")
    lines = file_handle.readlines()
    file_handle.close()
    
    total_count = 0
    file_handle = open(app_version_file, "w")
    for line in lines :
        new_line , count = re.subn(r'(.*APP_VERSION.*")(.*)(".*)', "\\1"+version+"\\3", line)        
        file_handle.write(new_line)
        total_count = total_count+count
    file_handle.close()
    
    if total_count <= 0 :
        raise Exception("cannot find APP_VERSION key")
        
def revert_file(file) :
    """回滚版本文件"""    
    git_revert_cmd = "git checkout \"%s\"" % file
    os.system(git_revert_cmd)

if __name__ == "__main__" :
    add_git_to_path()
    add_7z_to_path()
    git_update(base_dir )    

    version = get_version_string()
    print(version)
    
    #发布包名称
    package_name = "driverdll"+"_"+version
    
    #发布目录
    release_dir = os.path.abspath(script_dir+"..\\..\\target\\"+package_name)+"\\"
    os.makedirs(release_dir)
    
    try :    
        replace_app_version(base_dir+"include\\app_ver.h", version)
        compile_icr()
        copy_pdb_file(script_dir+"..\\Drivers\\lianzhi\\", release_dir+"..\\temp\\pdb\\")
        compress_pdb_file(release_dir+"..\\temp\\pdb\\", release_dir+"..\\"+package_name+".pdb.zip")        
        copy_release_file(release_dir)
        create_version_file(release_dir+"driverdll\\Drivers\\lianzhi\\version.txt", version)
        compress_full_release_file(release_dir, release_dir+"..\\"+package_name+".zip")
        revert_file(base_dir+"include\\app_ver.h")
        revert_all_change()
    except Exception as error :
        revert_file(base_dir+"include\\app_ver.h")
        revert_all_change()
        logging.exception(error)        
        exit(1)