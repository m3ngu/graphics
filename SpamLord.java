// CS124 HW1 SpamLord
import java.util.regex.*;
import java.util.Collections;
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.io.*;

public class SpamLord {

  /*
   * You do not need to modify anything in the Contact class.
   * This class encapsulates the basic information associated with
   * an e-mail or phone number for this assignment.  It has three
   * data members:
   *    filename // the name of the file in which the contact item was found
   *    type     // the type of contact information found: either "e" or "p"
   *    value    // the actual string representatino of the e-mail or phone number
   *             // see assignment description for details
   * you can ignore the other functions which are just necessary for correct
   * behavior when used an element of a java.uitl.Set
   */
  class Contact implements Comparable<Contact>{
    private String fileName;
    private String type;
    private String value;
    public Contact() {}
    public Contact(String fileName,String type,String value) {
      this.fileName = fileName;
      this.type = type;
      // automatically change value to lower case upon construction;
      this.value = value.toLowerCase();
    }
    public String getFileName() {return fileName;}
    public String getType() {return type;}
    public String getValue() {return value;}
    @Override
    public boolean equals(Object o) {
      Contact c = (Contact) o;
      return (fileName.equals(c.fileName) && type.equals(c.type) && value.equals(c.value));
    }
    @Override
    public int hashCode() {
      return 31*fileName.hashCode() + 17*type.hashCode() + value.hashCode();
    }
    public int compareTo(Contact c) {
      int fileNameCmp = fileName.compareTo(c.fileName);
      if (fileNameCmp != 0) {
        return fileNameCmp;
      }
      int typeCmp = type.compareTo(c.type);
      if (typeCmp != 0) {
        return typeCmp;
      }
      return value.compareTo(c.value);
    }
    @Override public String toString() {
      return fileName + "\t" + type + "\t" + value;
    }
  }
  
  // Example pattern for extracting e-mail addresses
  private static String add_spec = "[_A-Za-z0-9-]+(\\.[_A-Za-z0-9-]+)*(\\-[_A-Za-z0-9-]+)*(\\s+)?" +
  "(@|\\s+WHERE\\s+|\\s+at\\s+|&#x40;)(\\s+)?\\-?[A-Za-z0-9]+(\\-[A-Za-z0-9]+)*((\\.|\\s+DOM\\s+|\\s+do{0,1}t\\s+|;)[A-Za-z0-9]+)*\\-?(\\.|\\s+DOM\\s+|\\s+do{0,1}?t\\s+|;)(\\-?[A-Za-z]+)+";
  private Pattern myFirstPattern;
  private static String local_part = "[_A-Za-z0-9-]+(\\.[_A-Za-z0-9-]+)*(\\-[_A-Za-z0-9-]+)*";
  private static String global_part = "(\\s+)?\\-?[A-Za-z0-9]+(\\-[A-Za-z0-9]+)*((\\.|\\s+DOM\\s+|\\s+dot\\s+)[A-Za-z0-9]+)*\\-?(\\.|\\s+DOM\\s+|\\s+dot\\s+)(\\-?[A-Za-z]+)+";
  private static String ofuscate = "obfuscate\\((\\s+)?\\'?("+global_part+")\\'?(\\s+)?,(\\s+)?\\'?("+local_part+")\\'?(\\s+)?\\)";
  private static String nameFollowedBy = "("+local_part+")\\s+\\(followed\\s+by\\s+('|&ldquo;)@("+global_part+")('|&rdquo;)(\\s+)?\\)";
  Pattern pat = Pattern.compile(nameFollowedBy);
  private Pattern secondPattern;

  
  //PHONE NUMBERS
  private static String phone_numbers = "\\+?0*1?\\(?(?:\\s+)?([0-9]{3})(?:\\s+)?(?:(?:\\)(?:\\s+)?)|-|(?:\\s))([0-9]{3})(?:\\s|-)([0-9]{4})";
  private static Pattern phonePattern = Pattern.compile(phone_numbers);
  /* 
   * TODO
   * This should return a list of Contact objects found in the input. 
   * You can change anything internal to this function but make sure you
   * leave the interface (arguments and return value) unchanged because
   * it will be directly called by the submission script.
   */ 
	  
  private void storePhoneNumbers(String fileName, String line, List<Contact> contacts) {
	  Matcher m = phonePattern.matcher(line);
	  String phone;
	  while(m.find()) {
		  phone = m.group();
		  String commonCode = m.group(1);
		  String areaCode = m.group(2);
		  String number = m.group(3);
		  phone = commonCode.trim()+"-"+areaCode.trim()+"-"+number.trim();
		  Contact contact = new Contact(fileName,"p",phone);
          contacts.add(contact);
	  }
  }
  
  public List<Contact> processFile(String fileName, BufferedReader input) {
    List<Contact> contacts = new ArrayList<Contact>();
    // for each line
    Matcher m;
    String email;
    try {
      for(String line = input.readLine(); line != null; line = input.readLine()) {
        m = myFirstPattern.matcher(line);
        line = line.replaceAll("\"", "'");
        boolean matched = false;
        int probability = 0;
        boolean toCheck = false;
        boolean discard = false;
        while(m.find()) {
          email = m.group();
          
          if(!email.contains("@") && (email.toLowerCase().contains("where") ||
        		  email.toLowerCase().contains("at"))) {
        	  probability ++;
        	  toCheck = true;
          }
          if(!email.contains(".") && email.toLowerCase().contains("dom") || 
        		  email.toLowerCase().contains("dot") || email.toLowerCase().contains("dt")) {
        	  probability ++;
        	  toCheck = true;
          }
          
          //If it contains both where/at and dom/dot
          if(probability < 2 && toCheck) {
        	  //Possibility of false negatives
        	  //if the line contains neither mail nor email - 
        	  if(!line.contains("mailto") && !line.contains("email") && !line.contains("mail\\s+to")) {
        		  //Check if it contains html tags and if the end of the email is next to one
        		  if(!line.contains("<") || !line.contains(">")) {
        			  discard = true;
        		  }else {
        			  int index = line.indexOf(email);
        			  int lastIndex = index + email.length();
        			  String lineSubstring = line.substring(lastIndex);
        			  String[] splitStrings = lineSubstring.split("\\s+");
        			  
        			  if(splitStrings != null && splitStrings.length > 0) {
        				  int i = 0;
        				  for(i =0; i < splitStrings.length; i++) {
        					  if(splitStrings[i] != null && !splitStrings[i].equalsIgnoreCase("")) {
        						  break;
        					  }
        				  }
        				  if(i == splitStrings.length) {
        					  i --;
        				  }
        				  if(!splitStrings[i].equals("") && (!splitStrings[i].contains("<") || !splitStrings[i].contains(">"))) {
        					  discard = true;
        				  }
        			  }
        		  }
        	  }
          }
          if(discard) {
        	continue;  
          }
          
          if(!email.contains("@") && email.toUpperCase().contains("WHERE")) {
        	  email = email.replaceAll("\\s+WHERE\\s+", "@");
        	  email = email.replaceAll("\\s+where\\s+", "@");
          }else if (!email.contains("@") && email.toUpperCase().contains("AT")) {
        	  email = email.replaceAll("\\s+AT\\s+", "@");
        	  email = email.replaceAll("\\s+at\\s+", "@");
          }else if (!email.contains("@") && email.toLowerCase().contains("&#x40;")) {
        	  email = email.replaceAll("&#x40;", "@");
        	  email = email.replaceAll("&#X40;", "@");
          }
          email = email.replaceAll(";", ".");
          email = email.replaceAll("\\s+DOM\\s+", ".");
          email = email.replaceAll("\\s+dom\\s+", ".");
          email = email.replaceAll("\\s+DOT\\s+", ".");
          email = email.replaceAll("\\s+dot\\s+", ".");
          email = email.replaceAll("\\s+dt\\s+", ".");
          email = email.replaceAll("\\s", "");
          email = email.replaceAll("-", "");
          Contact contact = new Contact(fileName,"e",email);
          contacts.add(contact);
          matched = true;
        }
        //obfuscate
        if(!matched) {
        	m = secondPattern.matcher(line);
        	while(m.find()) {
        		String domain = m.group(2);
        		String name = m.group(11);
        		email = name+"@"+domain;
        		email = email.replaceAll("\\s+", "");
        		Contact contact = new Contact(fileName,"e",email);
                contacts.add(contact);
                matched = true;
        	}
        }
        //followed by
        if(!matched) {
        	m = pat.matcher(line);
        	while(m.find()) {
        		String domain = m.group(5);
        		String name = m.group(1);
        		email = name+"@"+domain;
        		email = email.replaceAll("\\s+", "");
        		Contact contact = new Contact(fileName,"e",email);
                contacts.add(contact);
                matched = true;
        	}
        }
        
        storePhoneNumbers(fileName, line, contacts);
      }
      input.close();
    } catch(IOException e) {
      e.printStackTrace();
      System.exit(1);
    }
    return contacts;
  }

  /*
   * You should not need to edit this, nor should you alter it's interface
   * because it will also be called direclty by the submission  program
   */
  public List<Contact> processDir(String dirName) {
    List<Contact> contacts = new ArrayList<Contact>();
    for(File f: new File(dirName).listFiles()) {
      if (f.getName().startsWith(".")) 
        continue;
      try {
        BufferedReader input = new BufferedReader(new FileReader(f));
        contacts.addAll(processFile(f.getName(), input));
      } catch(IOException e) {
        e.printStackTrace();
        System.exit(1);
      }
    }
    return contacts;
  }

  /*
   * You should not need to edit this function
   * It simply reads in a tsv gold file and returns a list of
   * Contacts
   */
  private List<Contact> loadGold(String goldPath) {
    List<Contact> gold = new ArrayList<Contact>();
    try {
      BufferedReader input = new BufferedReader(new FileReader(goldPath));
      
      String[] toks;
      for(String line = input.readLine(); line != null; line = input.readLine()) {
        toks = line.split("\t");
        Contact contact = new Contact(toks[0],toks[1],toks[2]);
        gold.add(contact);
      }
      input.close();

    } catch(IOException e) {
      e.printStackTrace();
      System.exit(1);
    }
    return gold;
  }

  /*
   * You should not need to edit this.
   * This is just a utility function which turns a Set into
   * a sorted list for convenience when looking at the output.
   */
  private List<Contact> asSortedList(Set<Contact> set) {
    Contact[] c = new Contact[0];
    List<Contact> list = Arrays.asList(set.toArray(c));
    Collections.sort(list);
    return list;
  }

  /*
   * You should not need to edit this.
   * This takes in two Lists of Contacts and prints out the intersection
   * and differences, which can be thought of as true positives, false
   * positives and false negatives.
   */
  private void score(List<Contact> guesses, List<Contact> gold) {
    Set<Contact> guess_set = new HashSet<Contact>();
    guess_set.addAll(guesses);
    Set<Contact> gold_set = new HashSet<Contact>();
    gold_set.addAll(gold);

    Set<Contact> tp = new HashSet<Contact>(guess_set);
    System.out.println("guess_set.size()="+guess_set.size()+"\tgold_set.size()="+gold_set.size());
    tp.retainAll(gold_set);
    List<Contact> tp_list = asSortedList(tp);
    System.out.println("True Positives (" + tp_list.size() +")\t###############################");
    for (Contact contact : tp_list) {
      System.out.println(contact);
    }

    Set<Contact> fp = new HashSet<Contact>(guess_set);
    fp.removeAll(gold_set);
    List<Contact> fp_list = asSortedList(fp);
    System.out.println("False Positives (" + fp_list.size() +")\t###############################");
    for (Contact contact : fp_list) {
      System.out.println(contact);
    }

    Set<Contact> fn = new HashSet<Contact>(gold_set);
    fn.removeAll(guess_set);
    List<Contact> fn_list = asSortedList(fn);
    System.out.println("False Negatives (" + fn_list.size() +")\t###############################");
    for (Contact contact : fn_list) {
      System.out.println(contact);
    }

    System.out.println("Summary: tp=" + tp.size() + "\tfp=" + fp.size() + "\tfn=" + fn.size());
  }

  /* 
   * main takes a directory and a file with the Gold contacts.
   * it processes each file in the directory, extracting any contacts
   * and compares them to the contacts listed in the gold file
   */
  public static void main(String[] args) {
    if (args.length != 2) {
      System.err.println("usage:\tSpamLord <data_dir> <gold_file>");
      System.exit(0);
    }
    SpamLord vader = new SpamLord();
    vader.myFirstPattern = Pattern.compile(add_spec);
    vader.secondPattern = Pattern.compile(ofuscate);
    List<Contact> guesses = vader.processDir(args[0]);
    List<Contact> gold = vader.loadGold(args[1]);
    vader.score(guesses,gold);
  }
}
