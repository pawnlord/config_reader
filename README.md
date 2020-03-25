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
```  
config cfg;  
auto_cfg_setup(cfg);  
```  
next, you have to read into the config with the file name of your config.
```
config_reader("config.cfg", cfg); // read file "config.cfg" into cfg  
```  
If you want a specific field, although unnecessary for getting attributes, you can get it like  
```  
char field[255][255] = {""}; // make sure field is completely empty. field also must have the second size be 255  
get_field(cfg, "FIELD1", field); // from config cfg, put contents under "[FIELD1]" in the config file into field  
```  
To read this field, use one of these commands:  
```  
char vals[50][50][50] = {{""}}; // last 2 must be 50  
get_attr(cfg, field, "MIN", vals) // From field, using EOL from cfg, find all values of MIN, put all instances of min with each value into vals
```   
Sometimes you just want one for either readability or memory reasons, for that you can get either the first or last using
```
get_first_attr(cfg, field, attr_name, val);
get_last_attr(cfg, field, attr_name, val);
```
Finally, you might not want to get a field as that might decrease readablity or increase memory usage. for this, you can use the direct commands:
```
dir_get_attr(cfg, field_name, attr_name, vals);`
dir_get_first_attr(cfg, field_name, attr_name, val);
dir_get_last_attr(cfg, field_name, attr_name, val);
```
  