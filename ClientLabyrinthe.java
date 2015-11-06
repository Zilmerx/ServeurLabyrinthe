package clientlabyrinthe;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.*;
import java.net.*;

/**
 *
 * @author 201171662
 */
public class ClientLabyrinthe {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) 
    {
        InetSocketAddress adresseSocket = null;
        Socket socket = null;
        
        DataOutputStream writer = null;
        BufferedReader reader = null;
        
        try
        {
            // Connexion
            adresseSocket = new InetSocketAddress("127.0.0.1", 4321);
            socket = new Socket();
            
            socket.connect(adresseSocket);
            
            OutputStream out = socket.getOutputStream();
            writer = new DataOutputStream (out);
            
            reader = new BufferedReader(
                            new InputStreamReader(
                                socket.getInputStream()));
            
            // Usage...
            BufferedReader clavier = new BufferedReader(
                                            new InputStreamReader(System.in));
            boolean fini = false;
            String texteBackground = null;
            String texteClavier = null;
            
            while(!fini)
            {
                //texteBackground = reader.readLine();
                
                // texteBackground afficher.
                
                texteClavier = clavier.readLine();
                
                if(texteClavier != null)
                {
                    texteClavier = texteClavier.toLowerCase();
                    if(texteClavier.equals("d") 
                            || texteClavier.equals("g") 
                            || texteClavier.equals("1"))
                    {
                        writer.write(texteClavier.getBytes());
                        writer.write('#');
                        
                        int cpt = 0;
                        char readc = (char)reader.read();
                        while(readc != '@')
                        {
                            System.out.print(cpt);
                            while(readc != '#')
                            {
                                System.out.print(readc);
                                readc = (char)reader.read();
                            }
                            System.out.println();
                            cpt++;
                            readc = (char)reader.read();
                        }
                    }
                }
                else
                {
                    fini = true;
                }
            }
            
            writer.close();
            reader.close();
            clavier.close();
            
            socket.close();
        }
        catch( IOException ioe )
        {
            System.err.println(ioe);
            System.exit(1);
        }
        
    }
    
    
    
}
