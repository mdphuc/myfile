<head>
  <link rel="stylesheet" href="index.css">
</head> 

<?php
    session_start();
?>

<table id="table_user" style='margin-top:50px'>
    <tr>
        <th>Họ và tên</th>
        <th>Tên đăng nhập</th>
        <th>Email</th>
        <th>Điện thoại</th>
        <th>Avatar</th>
    </tr>
    <tr>
        <?php 
            require_once "./db.php";
            $user = $_GET['user'];
            // echo $user;
            $db = new DB();
            $db->connect();
            $user_detail = $db->detail_user($user);
            $db->close();            
            foreach ($user_detail as $i => $v){
                if ($i != 2){
                    echo "<td>$v</td>";
                }
            }
        ?>
    </tr>
</table>

<br>

<div style='width:680px; border: 1px solid black; border-radius:5px; margin: 0 auto; padding: 10px; margin-top:30px'>
    <?php 
        $db = new DB();
        $db->connect();
        $comments = $db->get_comment($user);
        $db->close();  
        foreach ($comments as $i => $c){
            echo "<input type='hidden' id='$c[0]'>";
            echo "<div onmouseenter='reveal_modify_wizard($c[0])' onmouseleave='hide_modify_wizard($c[0])'><b>$c[1]</b>: $c[2]<button onclick='reveal(edit_comment_$c[0])' style='margin-left:10px; display:none' id='edit_$c[0]'>Sửa</button><button onclick='hide(edit_comment_$c[0])' style='margin-left:5px; background-color:#008CBA; display: none' id='hide_$c[0]'>Ẩn</button><form method='post'><input type='submit' value='Xóa' name='delete_comment_submit' style='background-color:#f44336; display: none' id='delete_$c[0]'><input name='id' type='hidden' value='$c[0]'></form><form name='edit_comment_$c[0]' style='display:none' method='post' id='edit_comment_submit_$c[0]'><span><input name='edit_comment_input'><input name='id' type='hidden' value='$c[0]'><input value='Update' type='submit' name='edit_comment_submit' style='background-color:#008CBA; margin-left:10px'></span></form></div>";
            // echo "<form name='edit_comment_$c[0]' style='display:none' method='post' id='edit_comment_submit_$c[0]'><span><input name='edit_comment_input'><input name='id' type='hidden' value='$c[0]'><input value='Update' type='submit' name='edit_comment_submit' style='background-color:#008CBA; margin-left:10px'></span></form>";
        }          
    ?>
</div>

<form method="post" id="control"></form>

<script>
    function reveal(target){
        target.style.display = 'block'
    }
    function hide(target){
        target.style.display = 'none'
    }
</script>

<script>
    function reveal_modify_wizard(target){
        document.getElementById("edit_" + target).style.display = 'inline-block'
        document.getElementById("hide_" + target).style.display = 'inline-block'
        document.getElementById("delete_" + target).style.display = 'inline-block'
    }
    function hide_modify_wizard(target){
        document.getElementById("edit_" + target).style.display = 'none'
        document.getElementById("hide_" + target).style.display = 'none'
        document.getElementById("delete_" + target).style.display = 'none'
        document.getElementById("edit_comment_submit_" + target).style.display = 'none'
    }
</script>

<?php
    if (isset($_POST["edit_comment_submit"])){
        $db->connect();
        $db->edit_comment($_POST["id"], $_POST["edit_comment_input"]);
        $db->close();  
        header("Refresh:0");  
    }
?>

<?php
    if (isset($_POST["delete_comment_submit"])){
        $db->connect();
        $db->delete_comment($_POST["id"]);
        $db->close();  
        header("Refresh:0");  
    }
?>

<form method='post'  id='comment_form' style="text-align:center; margin-top:30px">
    <textarea style='width:700px; height:400px' name='comment'></textarea>
    <br><br>
    <input type='submit' name='send_comment' value='Gửi' style='background-color: #04AA6D'>
</form>

<?php
    if (isset($_POST["send_comment"])){
        $db->connect();
        $send_from = $_SESSION["user"];
        $db->add_comment($user, $send_from, $_POST["comment"]);
        $db->close();  
        header("Refresh:0");
    }
?>

<br><br>

<form method="post">
    <input type="submit" name="home" value="Home">
</form>

<?php
    if (isset($_POST["home"])){
        header("Location: ./index.php");
    }
?>