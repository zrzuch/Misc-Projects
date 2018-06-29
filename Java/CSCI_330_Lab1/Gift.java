
/**
 * Filename		: Gift.java
 * Authors		: Alex Reardon & Zachary Zuch
 * Course		: CSCI 330
 * Assignment	: Gift Program
 * Description	: This program is responsible for generating a cart for 
 * 				  the user using a given file and offers a finite list of 
 * 				  available commands for the user to execute.
*/
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Gift
{
	private static int invAmount = 0; // count for inventory
	private static int count = 0;
	private static double total = 0.0;

	private static List<Integer> invItemNumber =
			new ArrayList<Integer>();
	private static List<Double> invPrices = new ArrayList<Double>();
	private static List<String> invItemDescr =
			new ArrayList<String>();

	private static List<Integer> userItemNumber =
			new ArrayList<Integer>();
	private static List<Double> userPrices = new ArrayList<Double>();
	private static List<String> userItemDescr =
			new ArrayList<String>();

	public static void main(String [] args)
	{
		if (args.length > 0)
		// Added this in to specify to add a parameter for the inventory file
		{
			try
			{
				generateInventory(args[ 0 ]);
				if (invAmount > 0)
				{
					query();
				}
				else
				{
					System.out.println(
							"Inventory File Must Include Items");
				}
			}
			catch ( IOException e )
			{
				System.out.println("Invalid Inventory File");
			}
		}
		else
		{
			System.out.println(
					"Include A Path Name Of An Inventory File");
		}
	}

	/**
	 * Reads the information from the given file and generates the inventory
	 * lists.
	 * 
	 * @param fileName
	 *            name of the file to read from
	 * @throws IOException
	 */
	public static void generateInventory(String fileName)
			throws IOException
	{
		Scanner fileReader = new Scanner(new File(fileName));
		while (fileReader.hasNextLine())
		{
			Scanner lineReader = new Scanner(fileReader.nextLine());
			/* 
			 * Didnt use condition hasNextDouble since I want it to fail when 
			 * it doesnt have a next double to print out the catch message.
			 */
			double price = lineReader.nextDouble();
			if (price > 0 && price < 1000)
			{
				invPrices.add(price);
			}
			else // I use else to also give 1.0 for negatives
			{
				invPrices.add(1.0);
			}
			String desc = lineReader.nextLine().substring(1);
			lineReader.close();
			if (desc.length() > 40)
			{
				desc = desc.substring(0, 40);
			}
			invItemDescr.add(desc);
			invItemNumber.add(invAmount++);
		}
		fileReader.close();
	}

	/**
	 * This method asks for the input and calls the corresponding method.
	 */
	public static void query()
	{
		Scanner console = new Scanner(System.in);
		String input = "";
		String header =
				"Hello! Please enter a command to continue.\n";
		String message = "[I] to see a list of available items.\n"
				+ "[A] to add an item to your basket.\n"
				+ "[B] to see what is in your basket.\n"
				+ "[C] to checkout with the current basket.\n"
				+ "Enter A Command: ";
		System.out.println(header);
		while (!input.equalsIgnoreCase("C"))
		{
			System.out.print(message);
			console.reset();
			input = console.nextLine();
			if (input.equalsIgnoreCase("A"))
			{
				addItems(console);
			}
			else if (input.equalsIgnoreCase("B"))
			{
				formatDisplay(userItemNumber.toArray(),
						userItemDescr.toArray(),
						userPrices.toArray());
			}
			else if (input.equalsIgnoreCase("C"))
			{
				checkOut();
			}
			else if (input.equalsIgnoreCase("I"))
			{
				formatDisplay(invItemNumber.toArray(),
						invItemDescr.toArray(), invPrices.toArray());
			}
			else
			{
				System.out.println(
						"Invalid Command. Please Try Again.");
			}
		}
		console.close();
	}

	/**
	 * This method is responsible for adding an item to the basket.
	 * 
	 * @param console
	 *            scanner that is used to ask for the item number
	 */
	public static void addItems(Scanner console)
	{
		System.out.print("\nEnter An Item Number: ");
		String itemNum = console.nextLine();
		/*
		 * I used nextLine() because in the specs Hutchens said we should to 
		 * flush out user errors
		*/
		try
		{
			int n = Integer.parseInt(itemNum);
			if (n >= 0 && n < invAmount)
			{
				userItemNumber.add(n);
				userPrices.add(invPrices.get(n));
				userItemDescr.add(invItemDescr.get(n));
				total += invPrices.get(n);
				++count;
			}
			else
			{
				System.out.println("No Item Number Matches");
			}
		}
		catch ( Exception e )
		{
			System.out.println("Enter A Number. Try Again.");
		}
		System.out.println();
	}

	/**
	 * This method displays the basket contents and the total.
	 */
	public static void checkOut()
	{
		System.out.printf(
				"\nThank you for your order. Your order contains: \n"
						+ "%39s          Price\n",
				"Order Item Description");

		System.out.printf("%55s\n",
				"----------------------------------------   -------");

		for (int i = 0; i < count; i++)
		{
			System.out.printf("%-2d  %-40s  %7.2f\n", i,
					userItemDescr.get(i), userPrices.get(i));
		}

		System.out.printf("\nThere are " + count
				+ " items. Your total cost is: $%.2f\n", total);
	}

	/**
	 * This method formats the output for the basket.
	 * 
	 * @param itemNum
	 *            array for the item numbers
	 * @param desc
	 *            array for the descriptions
	 * @param price
	 *            array for the prices
	 */
	public static void formatDisplay(Object [] itemNum,
			Object [] desc, Object [] price)
	{
		System.out.printf(
				"\n%-5s %11s %-28s %8s"
						+ "\n-------------------------------------------------------\n",
				"Item#", "", "Item Description", "Price");
		for (int i = 0; i < price.length; i++)
		{
			System.out.printf("%-5d %-40s %8.2f \n", itemNum[ i ],
					desc[ i ], price[ i ]);
		}
		System.out.println();
	}
}
