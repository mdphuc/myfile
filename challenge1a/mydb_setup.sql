CREATE DATABASE mydb;
CREATE TABLE `accounts` (
  `username` varchar(50) DEFAULT NULL,
  `password` varchar(50) DEFAULT NULL,
  `fullname` varchar(50) NOT NULL,
  `email` varchar(50) DEFAULT NULL,
  `avatar` blob DEFAULT NULL,
  `role` varchar(10) DEFAULT NULL,
  `phonenumber` varchar(300) DEFAULT NULL,
    PRIMARY KEY(`fullname`)
);

INSERT INTO `accounts` (`username`, `password`, `fullname`, `email`, `avatar`, `role`, `phonenumber`) VALUES
('teacher2', '123456a@A', 'Do Van B', 'teacher2@gmail.com', NULL, 'teacher', '0194357849'),
('student2', '123456a@A', 'Le Nhu D', 'student2@gmail.com', NULL, 'student', '0567382197'),
('student1', '123456a@A', 'Mai Van C', 'student1@gmail.com', NULL, 'student', '0455478493'),
('teacher1', '123456a@A', 'Nguyen Van A', 'teacher1@gmail.com', NULL, 'teacher', '0865478493');

CREATE TABLE `assignment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) DEFAULT NULL,
  `description` varchar(200) DEFAULT NULL,
  `type` varchar(30) DEFAULT NULL,
  `filename` varchar(50) DEFAULT NULL,
  `file` longblob DEFAULT NULL,
    PRIMARY KEY(`id`)
);

CREATE TABLE `comment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `send_to` varchar(50) DEFAULT NULL,
  `send_from` varchar(50) DEFAULT NULL,
  `message` varchar(50) DEFAULT NULL,
    PRIMARY KEY(`id`)
);

CREATE TABLE `submission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `submit_to` varchar(50) DEFAULT NULL,
  `file` longblob DEFAULT NULL,
  `type` varchar(30) DEFAULT NULL,
    PRIMARY KEY(`id`)
)