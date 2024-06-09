<?php 
    session_start();
    if (isset($_SESSION['user'])){
        header("Location: index.php");
    }
?> 
<head>
  <link rel="stylesheet" href="index.css">
</head> 
<body>
    <div class="login">
        <form action="login.php" method="post" style="margin-top:18%">
            <p>Username</p><input type="text" name="username">
            <p>Password</p><input type="password" name="password">
            <br><br>
            <input type="submit" name="login" value="Login">
        </form>
    </div>
    <?php
        if (isset($_POST["login"])){
            require_once './db.php';
            $db = new DB();
            $db->connect();
            $username = $_POST["username"];
            $password = $_POST["password"];
            $check = $db->check_user($username, $password);
            if ($check[1] == TRUE){
                session_start();
                $_SESSION['user'] = $check[0];
                header("Location: index.php");
            }
            $db->close();
        }
    ?>
</body>

