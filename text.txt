<?php
session_start();
$servername = "localhost";
$username = "elixir11user";
$password = "test11pass";
$database = "elixir_fashion11";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);

// Check connection
if (!$conn) {
die("Connection failed: " . mysqli_connect_error());
}

$ok_login=0;
if (isset($_SESSION['login']))
	if ($_SESSION['login']==1)
		$ok_login=1;
if ($ok_login==1)
{
	$results = mysqli_query($conn,"SELECT * FROM users WHERE id='".$_SESSION['userID']."';");

	if (mysqli_num_rows($results)) {
		$user = mysqli_fetch_array($results);
		$ok_login=1;
	}
	else
		$ok_login=0;
}
define("LOGGED_IN",$ok_login);
define("BE_SECURE",2); //0 - not at all, 1- yes, 2-by chance or set by a better constant
define("BE_SECURE_LOGIN",0);
define("LOGIN_USE_USERNAME",1);
define("LOGIN_USE_MD5",1);

function be_secure($case) {
	if (BE_SECURE==2)
		{
		if (constant($case)==1)
			return 1;
			else if (constant($case)==0)
				return 0;
				else
				{
				return 0;
				}
		}
	else
		return BE_SECURE;
}
