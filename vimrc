set nocompatible
filetype off
set rtp+=~/.vim/bundle/Vundle.vim

call vundle#begin()

" github 仓库中的插件 "
Plugin 'VundleVim/Vundle.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'taglist.vim'
Plugin 'majutsushi/tagbar' " Tag bar"
Plugin 'Xuyuanp/nerdtree-git-plugin'
Plugin 'jistr/vim-nerdtree-tabs'
Plugin 'vim-airline/vim-airline' | Plugin 'vim-airline/vim-airline-themes' " Status line"
Plugin 'jiangmiao/auto-pairs'
Plugin 'scrooloose/nerdcommenter'
"Plugin 'Valloric/YouCompleteMe'


call vundle#end()



set nu "设置显示行号
set backspace=2 "能使用backspace回删
syntax on "语法检测
set ruler "显示最后一行的状态
set bg=dark "背景色设置
set hlsearch "高亮度反白
set laststatus=2 "两行状态行+一行命令行
set cindent "设置c语言自动对齐
set t_Co=256 "指定配色方案为256
set mouse=a "设置可以在VIM使用鼠标
set tabstop=4 "设置TAB宽度
set history=1000 "设置历史记录条数

"快捷键:使用F3打开关闭
map <F3> :NERDTreeMirror <CR> 
map <F3> :NERDTreeToggle <CR>  
"NERDTree配置  
let NERDChristmasTree=1 "显示增强
let NERDTreeAutoCenter=1 "自动调整焦点
let NERDTreeShowFiles=1 "显示文件
let NERDTreeShowLineNumbers=1 "显示行号
let NERDTreeHightCursorline=1 "高亮当前文件
let NERDTreeShowHidden=0 "显示隐藏文件
let NERDTreeMinimalUI=0 "不显示'Bookmarks' label 'Press ? for help'
let NERDTreeWinSize=31 "窗口宽度

"配置Supertab
let g:SuperTabRetainCompletionType=2 "记住上次的补全方式,直到按Esc退出插入模式位置
let g:SuperTabDefaultCompletionType="<c-x><c-o>"  "按下tab后默认补全方式为<c-p>,现在改为<c-x><c-o>

"taglist配置信息
let Tlist_Auto_Open=1 " Let the tag list open automatically
let Tlist_Auto_Update=1 " Update the tag list automatically
let Tlist_Compact_Format=1 " Hide help menu
let Tlist_Ctags_Cmd='ctags' " Location of ctags
let Tlist_Enable_Fold_Column=0 "do show folding tree
let Tlist_Process_File_Always=1 " Always process the source file
let Tlist_Show_One_File=1 " Only show the tag list of current file
let Tlist_Exist_OnlyWindow=1 " If you are the last, kill yourself
"let Tlist_File_Fold_Auto_Close=0 " Fold closed other trees
let Tlist_Sort_Type="name" " Order by name
let Tlist_WinWidth=30 " Set the window 40 cols wide.
let Tlist_Close_On_Select=1 " Close the list when a item is selected
let Tlist_Use_SingleClick=1 "Go To Target By SingleClick
let Tlist_Use_Right_Window=1 "在右侧显示

" ctags 配置:F3快捷键显示程序中的各种tags，包括变量和函数等。
"let Tlist_Use_Left_Window=1                 "0:窗口在右边，1:窗口在左边，也可以改成Right_Window
let Tlist_Exit_OnlyWindow=1                 "如果只有一个buffer,kill窗口也kill掉buffer
let Tlist_Compart_Format = 1                "不显示空白行
let Tlist_File_Fold_Auto_Close = 1          "打开其他文件的时候自动关闭,只显示一个文件的tag
"let Tlist_Enable_Fold_Column = 0            "不要显示折叠树
"let Tlist_WinWidt=25

"打开关闭快捷键
map <silent> <F4> :TlistToggle<CR> 



"新建标签页
map <C-n> :tabnew <CR>  
map <C-c> :tabclose <CR>  
map <C-o> :tabonly <CR>  
map <C-h> :tabprevious <CR>  
map <C-l> :tabnext <CR>  

set tags=tags

"--------------------------------------------------------------------------------
"" cscope:建立数据库：cscope -Rbq；  F5 查找c符号； F6 查找字符串；   F7
"" 查找函数谁调用了，
"--------------------------------------------------------------------------------
if has("cscope")
  set csprg=/usr/bin/cscope
  set csto=1
  set cst
  set nocsverb
  " add any database in current directory
  if filereadable("cscope.out")
    cs add cscope.out
  endif
  set csverb
endif
":set cscopequickfix=s-,c-,d-,i-,t-,e-

"nmap <C-_>s :cs find s <C-R>=expand("<cword>")<CR><CR>
"F5 查找c符号； F6 查找字符串；  F7 查找函数谁调用了，
nmap <silent> <F5> :cs find s <C-R>=expand("<cword>")<CR><CR> :botright copen<CR><CR> 
nmap <silent> <F6> :cs find t <C-R>=expand("<cword>")<CR><CR> :botright copen<CR><CR>
nmap <silent> <F7> :cs find c <C-R>=expand("<cword>")<CR><CR> :botright copen<CR><CR>
