# user_demo

Welcome to the user_demo wiki!

This little project created a simple server to process the log in requests of the users, and verification the ID & password.

After log in the server, users can send mails to the server, and send mails to ech other users.

Every user can send commands to the server to get the mails, which send to him.

Users ID and messages should be stored in the database of MySQL.

The database has two tables, user table and mails table.

#### Example:

Using the name of the users as the mails address. Such as, someone want to send a mails to 'xlz' with 'aaaaaa' like this:

```
mail xlz aaaaaa
```
Then 'xlz' log in the server, and send the command like this to get mails:
```
getmail
```
Then the server will return the message:
```
1 aaaaaa
```


