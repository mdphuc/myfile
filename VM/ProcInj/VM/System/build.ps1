../Mingw64/msys2-x86_64-20240113.exe in --confirm-command --accept-messages --root C:/msys64
C:\msys64\usr\bin\bash.exe -l -c "pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain --noconfirm"
../Python3.9/python-3.9.10-amd64.exe InstallAllUsers=0 InstallLauncherAllUsers=0 PrependPath=1 Include_test=0

pip install -r ../Python3.9/requirements.txt

$og = (Get-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name path).path
$update = "$og;C:\msys64\ucrt64\bin"  

Set-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name path -Value $update

New-NetFirewallRule -DisplayName "Allow TCP 5500" -Direction Inbound -Action Allow -EdgeTraversalPolicy Allow -Protocol TCP -LocalPort 5500 

./setup.exe

