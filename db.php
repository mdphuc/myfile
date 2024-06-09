<?php 
    class DB{
        private $DEFAULT_PWD = "123456a@A";

        private $hostname = "localhost:3306";
        private $username = "root";
        private $password = "";
        private $db = "mydb";
        private $conn;
        
        function connect(){
            $conn = new mysqli($this->hostname, $this->username, $this->password, $this->db);
            if (!$conn){
                die("".mysqli_error($conn));
                exit();
            }else{
                $this->conn = $conn;
                return $conn; 
            }
        }

        function check_user($username, $password){
            $check_query = "SELECT * FROM accounts WHERE username='".$username."' AND password='".$password."'";
            $check_account = mysqli_query($this->conn, $check_query);
            $output = array();
            if (!$check_account){
                die("".mysqli_error($check));
                exit();
            }else{
                if (mysqli_num_rows($check_account) > 0){
                    array_push($output, mysqli_fetch_row($check_account)[2]);
                    array_push($output, TRUE);
                    return $output;
                }else{
                    array_push($output, "");
                    array_push($output, FALSE);
                    return $output;
                }
            }
        }

        function list_user(){
            $list_query = "SELECT fullname FROM accounts ";   
            $list_run = mysqli_query($this->conn, $list_query);

            $count = 0;
            $accounts = array();
            while($row_item = mysqli_fetch_row($list_run)){
                $row = array();
                foreach ($row_item as $i => $v){
                    array_push($row, $v);
                }
                array_push($accounts, $row);
            }   
            return $accounts;
        }

        function detail_user($user_fullname){
            $detail_user_query = "SELECT fullname, username, password, email, phonenumber, avatar FROM accounts WHERE fullname='{$user_fullname}'";   
            $detail_user_run = mysqli_query($this->conn, $detail_user_query);

            $count = 0;
            $user_ = array();
            $row_item = mysqli_fetch_row($detail_user_run);
            foreach ($row_item as $i => $v){
                array_push($user_, $v);
            }
            return $user_;
        }

        function get_comment($user_target){
            $get_comment_query = "SELECT id, send_from, message FROM comment WHERE send_to='".$user_target."'";  
            $get_comment_run = mysqli_query($this->conn, $get_comment_query);
            $comments = array();
            while($row_item = mysqli_fetch_row($get_comment_run)){
                $row = array();
                foreach ($row_item as $i => $v){
                    array_push($row, $v);
                }
                array_push($comments, $row);
            }   
            return $comments;
        }

        function add_comment($send_to, $send_from, $message){
            $add_message_query = "INSERT INTO comment (send_to, send_from, message) VALUES ('".$send_to."', '".$send_from."', '".$message."')";
            $add_message_run = mysqli_query($this->conn, $add_message_query);
        }

        function edit_comment($id, $edit_message){
            $edit_message_query = "UPDATE comment SET message='".$edit_message."' WHERE id='".$id."'";
            $edit_message_run = mysqli_query($this->conn, $edit_message_query);
        }

        function delete_comment($id){
            $delete_message_query = "DELETE FROM comment WHERE id='".$id."'";
            $delete_message_run = mysqli_query($this->conn, $delete_message_query);
        }

        function check_role($fullname){
            $role_query = "SELECT role FROM accounts WHERE fullname='".$fullname."'";
            $role = mysqli_fetch_row(mysqli_query($this->conn, $role_query))[0];
            return $role;
        }

        function list_student(){
            $list_student_query = "SELECT fullname, username, password, email, phonenumber, avatar FROM accounts WHERE role='student'";   
            $list_student_run = mysqli_query($this->conn, $list_student_query);

            $count = 0;
            $students = array();
            while($row_item = mysqli_fetch_row($list_student_run)){
                $row = array();
                foreach ($row_item as $i => $v){
                    array_push($row, $v);
                }
                array_push($students, $row);
            }   
            return $students;
        }

        function update_student($target, $field, $item){
            $update_student_query = "UPDATE accounts SET $field='".$item."' WHERE username='".$target."'";
            $update_student_run = mysqli_query($this->conn, $update_student_query);
        }

        function add_student($target, $field, $item){
            $current_item_query = "SELECT $field FROM accounts WHERE username='".$target."'";
            $current_item_run = mysqli_fetch_row(mysqli_query($this->conn, $current_item_query))[0];

            $item = "$current_item_run, $item";

            $add_student_query = "UPDATE accounts SET $field='".$item."' WHERE username='".$target."'";
            $add_student_run = mysqli_query($this->conn, $add_student_query);
        }

        function remove_student($target){
            $delete_student_query = "DELETE FROM accounts WHERE username='".$target."'";
            $delete_student_run = mysqli_query($this->conn, $delete_student_query);
        }

        function upload_assignment($name, $description, $file_data, $file_type, $file_name){
            $upload_assignment_query = "INSERT INTO assignment (name, description, file, type, filename) VALUES ('{$name}', '{$description}', '{$file_data}', '{$file_type}', '{$file_name}')";

            $upload_assignment_run = mysqli_query($this->conn, $upload_assignment_query);
        }

        function list_assignment(){
            $list_assignment_query = "SELECT id, name, description, file FROM assignment";   
            $list_assignment_run = mysqli_query($this->conn, $list_assignment_query);

            $count = 0;
            $assignments = array();
            while($row_item = mysqli_fetch_row($list_assignment_run)){
                $row = array();
                foreach ($row_item as $i => $v){
                    if ($i != 3){
                        array_push($row, $v);
                    }
                }
                array_push($assignments, $row);
            }   

            return $assignments;
        }

        function download_file($id, $type){
            if ($type == "assignment"){
                $download_file_query = "SELECT type, file FROM assignment WHERE id='{$id}'";
            }else{
                $download_file_query = "SELECT type, file FROM submission WHERE id='{$id}'";
            }
            $download_file_run = mysqli_query($this->conn, $download_file_query);
            $file_object = mysqli_fetch_row($download_file_run);
            header("Content-type: ".$file_object[0]);
            echo $file_object[1];
        }

        function list_submission($assignment){
            $list_submission_query = "SELECT id, submit_to, file, type FROM submission WHERE submit_to='{$assignment}'";

            $list_submission_run = mysqli_query($this->conn, $list_submission_query);

            $count = 0;
            $submissions = array();
            while($row_item = mysqli_fetch_row($list_submission_run)){
                $row = array();
                foreach ($row_item as $i => $v){
                    if ($i != 2){
                        array_push($row, $v);
                    }
                }
                array_push($submissions, $row);
            }   
            
            // echo $submissions[0][0];
            return $submissions;


        }

        function submit_assignment($submit_to, $file_data, $file_type){
            $submit_assignment_query = "INSERT INTO submission (submit_to, file, type) VALUES ('{$submit_to}', '{$file_data}', '{$file_type}')";

            $submit_assignment_run = mysqli_query($this->conn, $submit_assignment_query);
        }

        function close(){
            mysqli_close($this->conn);
        }

    
    }

?>