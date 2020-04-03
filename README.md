

# config_reader  
This is a more personal project for when I want to read config files, something I have had to do on a case by case basis before.  
  
This is meant to be simple config files so that parameters don't get to intense. This is more meant for personal projects, such as the Tarrasch editor (which you can find [here](https://www.github.com/pawnlord/tarrasch-editor)).  
  
It's pretty simple, so here is the rundown  
  
## config file
A config file is broken down into two separate parts, fields and attributes  
A field is defined by square brackets, and I tend to use uppercase for them.  
An attribute is like a C style define but without the preprocessor and it can have multiple values.  
You can have multiple of the same attribute, but an array to hold this takes up a lot of memory.  
EOLs can be whatever you like, but you will have to change things in the code later on.  
Example of config file:
```
[FIELD1]
MIN 10
MAX 20
NAME John
BIRTHDAY 1 1 2000

[FIELD2]
MONEY 1234.56
CURRENCY Euro
```

## code
The code I would say is a bit more tricky.  
First, you have to make a config struct and setup  
```c  
config cfg;  
auto_cfg_setup(cfg);  
```  
next, you have to read into the config with the file name of your config.
```c
config_reader("config.cfg", cfg); // read file "config.cfg" into cfg  
```  
If you want a specific field, although unnecessary for getting attributes, you can get it like  
```  c
char** field; // make sure field is completely empty. all values should be set to 0.
/* code to make it empty . . . */
get_field(cfg, "FIELD1", field); // from config cfg, put contents under "[FIELD1]" in the config file into field  
```  
To read this field, use one of these commands:  
``` c 
char*** vals;
/* code to make it empty . . . */ 
get_attr(cfg, field, "MIN", vals) // From field, using EOL from cfg, find all values of MIN, put all instances of min with each value into vals
```   
Sometimes you just want one for either readability or memory reasons, for that you can get either the first or last using
```c
get_first_attr(cfg, field, attr_name, val);
get_last_attr(cfg, field, attr_name, val);
```
Finally, you might not want to get a field as that might decrease readablity or increase memory usage. for this, you can use the direct commands:
```c
dir_get_attr(cfg, field_name, attr_name, vals);`
dir_get_first_attr(cfg, field_name, attr_name, val);
dir_get_last_attr(cfg, field_name, attr_name, val);
```
  
# config editing
Currently, you must directly edit the config words if you want to edit it, but I will add functions for this soon!  
If you have edited it, there is a save_config function with syntax:  
```c
int save_config(config* cfg, char filename[]);
```  
so, use that as you will  

# reference sheet
This is all functions for quick reference!
This is pretty much what you will find in the header file.
```c
int file_reader(char* filename, char* buffer);

int config_reader(char* buffer, config* cfg);

int get_field(config cfg, char* field_name, char** field); 

int get_attr(config cfg, char** field, char* attr, char*** val); 

int get_first_attr(config cfg, char** field, char* attr, char** val); 

int get_last_attr(config cfg, char** field, char* attr, char** val);

int dir_get_attr(config cfg, char* field, char* attr, char*** val);

int dir_get_first_attr(config cfg, char* fieldname, char* attr, char** val);

int dir_get_last_attr(config cfg, char* fieldname, char* attr, char** val);

void set_cfg_eol(config* cfg, char eol);

void cfg_setup(config* cfg, char eol, char begin, char end);

void auto_cfg_setup(config* cfg);

int save_config(config* cfg, char* filename);

/* Unimplemented, but coming! */
int set_field_attr(config* cfg, char* fieldname, char* attrname, char** new_val);

int set_field(config* cfg, char* fieldname, char** new_field);

void set_cfg_field(config* cfg, char begin, char end); 

```