import java.io.*;
 
class CreateDir
{
    public static void main(String[] args) throws IOException
    {
        process("Simplified Chinese"  , "世界您好");
        process("Cyrillic"            , "азеазея");
        process("amharique"           , "ድቅስድቅስግ");
        process("Japanese"            , "ハロー・ワールド");
        process("Thai"                , "เฮลโลเวิลด์");
        process("Arabish"             , "حريات وحقوق");
        process("Korean"              , "프로그램");
        process("Vietanmien"          , "프로그램");
        process("Malien"              , "ഹലോ വേള്‍ഡ");
        process("Hebreu"              , "תוכנית");
    }
    
    public static void process(String language , String strToUse) throws IOException
    {
        try
        {
            String dirpath     = "dir_"+strToUse;
            String filepath    = "dir_"+strToUse+"/file_"+strToUse;
            String filecontent = "str_"+strToUse+" : "+language;
            
            File d = new File(dirpath);
            File f = new File(filepath);
            
            if( ! d.exists() )
            {
                d.mkdir();
            }
            
            if( ! f.exists() )
            {
                f.createNewFile();
            }
            
            OutputStreamWriter out = new OutputStreamWriter(new FileOutputStream(f) , "UTF-8");
            out.write(filecontent);
            out.close();
        }
        catch(Exception e)
        {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
