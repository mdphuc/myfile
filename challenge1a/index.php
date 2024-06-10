<?php 
    session_start();
    if (!isset($_SESSION['user'])){
        echo "<script>window.location.replace('https://mdphuc.000webhostapp.com/login.php')</script>";
    }else{
        echo "<h1>Dashboard</h1>";
        $current = $_SESSION['user'];
        require_once './db.php';
        $db = new DB();
        $db->connect();
        $role = $db->check_role($current);
        $db->close();
        if ($role == "teacher"){
            echo "Chào giáo viên <b>$current</b>";
        }else{
            echo "Chào học sinh <b>$current</b>";
        }    
    }
?>
<br>
<head>
  <link rel="stylesheet" href="index.css">
</head> 
<br><br>
<form method="post"><input type="submit" name="logout" value="Đăng xuất" style='background-color: #f44336'></form>
<form method="post">
    <input type="submit" name="your_info" value="Thông tin của bạn" style='background-color: #008CBA'>
</form>
<form method="post"><input type="submit" value="Danh sách người dùng" name="list_user"></form>

<?php
    if ($role == "teacher"){
        echo "<form method='post' id='list_student_form'><input type='submit' value='Danh sách học sinh' name='list_student'></form>";
        echo "<button onclick='reveal(upload_assignment_form)'>Giao bài tập</button>";
    }
?>

<?php
    if (isset($_POST["logout"])){
        session_destroy();
        echo "<script>window.location.replace('https://mdphuc.000webhostapp.com/login.php')</script>";
    }
?>
<br><br>
<form method="post"><input type="submit" value="Danh sách bài tập" name="list_assignment"></form>

<div>
    <form method='post' id='upload_assignment_form' style='display:none; text-align:center; margin-top:30px' enctype="multipart/form-data">
        <p>Tên</p>
        <input type='text' name="upload_name" style="width: 700px"><br>
        <p>Mô tả</p>
        <textarea type='text' name="upload_description" style="width: 700px; height: 400px"></textarea><br><br><br>
        <input type='file' name="upload_file"><br><br>
        <input type='submit' name='upload_assignment' value='Gửi' style="background-color: #04AA6D">
    </form>
</div>

<script>
    function reveal(target){
        target.style.display = 'block'
    }
</script>

<table id="table_user">
    <?php 
        if (isset($_POST["list_user"])){
            require_once './db.php';
            $db = new DB();
            $db->connect();
            $accounts = $db->list_user();
            $db->close();
            echo "<tr><th>Họ và tên</th></tr>";
            foreach ($accounts as $i => $v){
                $name = $v[0];
                if ($name != $_SESSION["user"]){
                    echo "<tr><td><a href='./detail.php?user=$name'>$name</a></td></tr>";
                }else{
                    echo "<tr><td><a href='./detail.php?user=$name' style='color:red'>$name</a></td></tr>";
                }
            }
        }
    ?>
</table>

<table id="table_yourself">
    <?php 
        if (isset($_POST["your_info"])){
            require_once './db.php';
            $db = new DB();
            $db->connect();
            $students = $db->detail_user($current);
            $s = $students;
            $db->close();
            echo "<tr><th>Họ và tên</th><th>Tên đăng nhập</th><th>Mật khẩu</th><th>Email</th><th>SĐT</th><th>Avatar</th><tr>";
            echo "<input type='hidden' id='edit'>";
            echo "<input type='hidden' id='add'>";
            echo "<input type='hidden' id='remove'>";
            echo "<input type='hidden' id='$s[1]'>";
            echo "<input type='hidden' id=$role>";
            echo "<tr onmouseenter='reveal_modify_wizard($s[1])' onmouseleave='hide_modify_wizard($s[1])'><td id='$s[1]_fullname'><a href='./detail.php?user=$s[0]'>$s[0]</a></td><td id='$s[1]_username'>$s[1]</td><td id='$s[1]_password'>$s[2]</td><td id='$s[1]_email'>$s[3]</td><td id='$s[1]_phonenumber'>$s[4]</td><td id='$s[1]_avatar'>$s[5]</td><td id='modify_wizard_$s[1]' style='display:none'><button onclick='reveal_edit_place($s[1], $role)'>Sửa</button><form method='post' id='$s[1]_form'><input value='Update' name='student_modify' type='hidden'></form><button onclick='student_modify($s[1], edit)' style='background-color: #008CBA'>Cập nhật</button><button onclick='student_modify($s[1], add)' style='margin-left:5px; background-color: #04AA6D'>Thêm</button><button onclick='student_modify($s[1], remove)' style='margin-left:5px; background-color: #f44336'>Xóa</button></td></tr>";
        }
    ?>

</table>

<?php
    if (isset($_POST["upload_assignment"])){
        $name = $_POST["upload_name"];
        $description = $_POST["upload_description"];
        $file_type = $_FILES["upload_file"]["type"];
        $file_data = addslashes(file_get_contents($_FILES["upload_file"]["tmp_name"]));
        $file_name = $_FILES["upload_file"]["name"];
        $db = new DB();
        $db->connect();
        $db->upload_assignment($name, $description, $file_data, $file_type, $file_name);
        $db->close();

    }
?>

<table id="table_student">
    <?php 
        if (isset($_POST["list_student"])){
            require_once './db.php';
            $db = new DB();
            $db->connect();
            $students = $db->list_student();
            $db->close();
            echo "<tr><th>Họ và tên</th><th>Tên đăng nhập</th><th>Mật khẩu</th><th>Email</th><th>SĐT</th><th>Avatar</th><tr>";
            echo "<input type='hidden' id='edit'>";
            echo "<input type='hidden' id='add'>";
            echo "<input type='hidden' id='remove'>";
            echo "<input type='hidden' id=$role>";
            foreach ($students as $i => $s){
                echo "<input type='hidden' id='$s[1]'>";
                echo "<tr onmouseenter='reveal_modify_wizard($s[1])' onmouseleave='hide_modify_wizard($s[1])'><td id='$s[1]_fullname'><a href='./detail.php?user=$s[0]'>$s[0]</a></td><td id='$s[1]_username'>$s[1]</td><td id='$s[1]_password'>$s[2]</td><td id='$s[1]_email'>$s[3]</td><td id='$s[1]_phonenumber'>$s[4]</td><td id='$s[1]_avatar'>$s[5]</td><td id='modify_wizard_$s[1]' style='display:none'><button onclick='reveal_edit_place($s[1], $role)'>Sửa</button><form method='post' id='$s[1]_form'><input value='Update' name='student_modify' type='hidden'></form><button onclick='student_modify($s[1], edit)' style='background-color: #008CBA'>Cập nhật</button><button onclick='student_modify($s[1], add)' style='margin-left:5px; background-color: #04AA6D'>Thêm</button><button onclick='student_modify($s[1], remove)' style='margin-top:5px; background-color: #f44336'>Xóa</button></td></tr>";
            }
        }
    ?>
</table>

<script>
    function reveal_modify_wizard(target){
        target = target.id;
        document.getElementById("modify_wizard_" + target).style.display = 'block'
        // alert(target)
    }
    function hide_modify_wizard(target){
        target = target.id;
        document.getElementById("modify_wizard_" + target).style.display = 'none'
        // alert(target)
    }
</script>

<table id="table_assignment">
    <?php 
        if (isset($_POST["list_assignment"])){
            require_once './db.php';
            $db = new DB();
            $db->connect();
            $assignments = $db->list_assignment();
            $db->close();
            echo "<tr><th>Tên</th><th>Mô tả</th><th>File</th><tr>";
            foreach ($assignments as $i => $a){
                if ($role == "teacher"){
                    echo "<input type='hidden' id='$a[1]'>";
                    echo "<tr><td>$a[1]</td><td>$a[2]</td><td><form method='post'><input type='hidden' value='$a[0]' name='id'><input type='hidden' value='assignment' name='type'><input type='submit' value='Tải về' name='file_download' style='background-color: #04AA6D'></form></td><td><button onclick='reveal_submission($a[1])' style='background-color: #008CBA'>Danh sách bài làm</button></td></tr>";
                }else{
                    echo "<input type='hidden' id='$a[1]'>";
                    echo "<tr><td>$a[1]</td><td>$a[2]</td><td><form method='post'><input type='hidden' value='$a[0]' name='id'><input type='hidden' value='assignment' name='type'><input type='submit' value='Tải về' name='file_download' style='background-color: #04AA6D'></form></td><td><button onclick='reveal_submission_place($a[1])' style='background-color: #008CBA'>Nộp bài</button></td></tr>";
                }
            }
        }
    ?>
</table>

<?php
    if (isset($_POST["file_download"])){
        $db = new DB();
        $db->connect();
        $db->download_file($_POST["id"], $_POST["type"]);
        $db->close();
    }
?>

<form method="post" id="reveal_submission_form"></form>
<br><br>
<form method='post' id='submission_form' style='display:none' enctype="multipart/form-data">
    <input type='file' name="submit_file"><br>
    <input type='submit' name='submit_assignment' value='Submit'>
</form>

<?php
    if (isset($_POST["submit_assignment"])){
        $file_type = $_FILES["submit_file"]["type"];
        $file_data = addslashes(file_get_contents($_FILES["submit_file"]["tmp_name"]));
        $submit_to = $_POST["target_submission"];
        $db = new DB();
        $db->connect();
        $db->submit_assignment($submit_to, $file_data, $file_type);
        $db->close();

    }
?>

<script>
    field = ["fullname", "username", "password", "email", "phonenumber", "avatar"]
    function reveal_edit_place(target, role){
        target = target.id
        role = role.id
        if (role == "teacher"){
            start = 0;
        }else if (role == "student"){
            start = 2;
        }
        for (let i = start; i < 5; i++){
            input = document.createElement('input')
            input.type = 'TEXT'
            input.id = field[i] + "_" + target + "_" + "edit"
            input.name = field[i] + "_" + "edit"
            document.getElementById(target + "_" + field[i]).appendChild(input)
        }

        input = document.createElement('input')
        input.type = 'FILE'
        input.id = field[5] + "_" + target + "_" + "edit"
        input.name = field[5] + "_" + "edit"
        document.getElementById(target + "_" + field[5]).appendChild(input)

    }
    function student_modify(target, type){
        target = target.id
        type = type.id
        student_update_form = document.getElementById(target + "_" + "form")
        if (type != "remove"){
            for (let i = 0; i < 6; i++){
                if (field[i] != "avatar"){
                    edit = document.getElementById(field[i] + "_" + target + "_" + "edit")
                    edit.type = 'hidden'
                    student_update_form.appendChild(edit)
                }
            }
            edit = document.getElementById('avatar' + "_" + target + "_" + "edit")
            edit.type = 'hidden'
            
            if (document.getElementById(target + '_' + 'avatar').value != "" && type == 'add'){
                edit.value = ""
            }

            edit_type = document.createElement('input')
            edit_type.type = 'hidden'
            edit_type.name = 'type'
            edit_type.value = type

            edit_target = document.createElement('input')
            edit_target.type = 'hidden'
            edit_target.name = "target"
            edit_target.value = target

            student_update_form.appendChild(edit)
            student_update_form.appendChild(edit_type)
            student_update_form.appendChild(edit_target)

            student_update_form.submit()
        }else{
            edit_type = document.createElement('input')
            edit_type.type = 'hidden'
            edit_type.name = 'type'
            edit_type.value = type

            edit_target = document.createElement('input')
            edit_target.type = 'hidden'
            edit_target.name = "target"
            edit_target.value = target

            student_update_form.appendChild(edit_type)
            student_update_form.appendChild(edit_target)

            student_update_form.submit()
        }
    }
    function reveal_submission(target){
        target = target.id
        // alert(target)
        reveal_submission_form = document.getElementById("reveal_submission_form")

        target_ = document.createElement('input')
        target_.type = 'hidden'
        target_.name = 'target_assignment'
        target_.value = target

        reveal_submission_form.appendChild(target_)

        reveal_submission_form.submit()
    }
    function reveal_submission_place(target){
        target = target.id
        submit_form = document.getElementById("submission_form")

        submit_to = document.createElement('p')
        submit_to.value = target

        target_ = document.createElement('input')
        target_.type = 'hidden'
        target_.name = 'target_submission'
        target_.value = target

        submit_form.appendChild(target_)
        submit_form.appendChild(submit_to)

        submit_form.style.display = 'block'
    }
</script>

<table id="table_submission">
    <?php 
        if (isset($_POST["target_assignment"])){
            require_once './db.php';
            $db = new DB();
            $db->connect();
            $submissions = $db->list_submission($_POST["target_assignment"]);
            $db->close();
            echo "<tr><th>Ten bai tap</th><th>File</th><tr>";
            foreach ($submissions as $i => $s){
                if ($role == "teacher"){
                    echo "<input type='hidden' id='$s[1]'>";
                    echo "<tr><td>$s[1]</td><td><form method='post'><input type='hidden' value='$s[0]' name='id'><input type='hidden' value='submission' name='type'><input type='submit' value='Tai ve' name='file_download'></form></td></tr>";
                }
            }
        }
    ?>
</table>

<?php
    if (isset($_POST["student_modify"])){
        $type = $_POST["type"];
        $target = $_POST["target"];

        if ($type == "edit"){
            if ($role == "teacher"){
                $field = array("fullname", "username", "password", "email", "phonenumber", "avatar");
            }else if ($role == "student"){
                $field = array("password", "email", "phonenumber", "avatar");
            }
            foreach ($field as $i => $f){
                if ($_POST["$field[$i]_edit"] != ""){
                    $db = new DB();
                    $db->connect();
                    $db->update_student($target, $f, $_POST["$field[$i]_edit"]);
                    $db->close();
                }
            }
        }

        if ($type == "add"){
            $field = array("email", "phonenumber", "avatar");
            foreach ($field as $i => $f){
                if ($_POST["$field[$i]_edit"] != "" && is_numeric($_POST["$field[$i]_edit"])){
                    $db = new DB();
                    $db->connect();
                    $db->add_student($target, $f, $_POST["$field[$i]_edit"]);
                    $db->close();
                }
            }
        }

        if ($type == "remove"){
            $db = new DB();
            $db->connect();
            $db->remove_student($target);
            $db->close();
        }
    }
?>
