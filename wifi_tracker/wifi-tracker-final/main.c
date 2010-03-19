/*Call Backs*/

#include "main.h"

char our_ip[20],helper_ip[20],track_ip[50][20],msgs[100][50];
int helper_dist=10, flag=0, no_of_ip=0;
int dist1[50],dist2[50];
int x_cord[50], y_cord[50],msg_count=0;

void itoa(int a,char *b)
{
	char c[4];
	int i=0,j=0;
	for(i=0; i<4; i++)
	{
		b[i]=c[i]='\0';
	}
	i=1;
	while (a>0)
	{
		c[i++]=a%10 + 48;
		a/=10;
	}
	for(i=i-1;i>=0;i--)
	{
		b[j++]=c[i];
	}
}

void showlabel7()
{
	int i=0;
	char display_msg[1000];
	strcpy(display_msg,"\0");
	for (i=0; i<msg_count; i++)
	{
		strcat(display_msg, msgs[i]);
		strcat(display_msg, "\n");
	}
	gtk_label_set_text(label7, display_msg);
}

void measure_strength_ip()
{

	/* Read the data from the file written by iwspy*/
	int found=0,db=0,i=0,j=0;
	char temp_ip[20],temp_ch,db_ch[5],time[50];
	GTimeVal secs;
	FILE *fp;
	fp = fopen("iwspy_data","r");
	temp_ch = fgetc(fp);
	
	if (temp_ch != '\"')
	{
		printf("Error in iwspy_data\n");
		strcpy(msgs[msg_count++],"Error in iwspy_data.\0");
		return;
	}
next:	i=0;
	do
	{
		temp_ch = fgetc(fp);
		if (temp_ch != '\"')
			temp_ip[i++]=temp_ch;
	}
	while(temp_ch != '\"');

	temp_ip[i]='\0';
	for(i=0; i<no_of_ip; i++)
	{

		if (strcmp(temp_ip,track_ip[i])==0)
		{
			found++;
			break;
		}
	}
	if (found==0)
	{
		//Found a new one
		//Alert
		g_get_current_time(&secs);
		
		strcpy(track_ip[no_of_ip++],temp_ip);
		printf("Found a new ip at %s\n",g_time_val_to_iso8601(&secs));
		//strcpy(msgs[msg_count],g_time_val_to_iso8601(&secs));
		strcat(msgs[msg_count],"Found a new ip. ");
		strcat(msgs[msg_count],track_ip[no_of_ip-1]);
		strcat(msgs[msg_count++],"\0");
		
	}	
	
	temp_ch = fgetc(fp);
	temp_ch = fgetc(fp);
	db=0;
	while(1)	
	{
		db = (db * 10) + (int) temp_ch - 48 ;	
		temp_ch = fgetc(fp);
		if (temp_ch=='d')
		{
			break;
		}
	}

	//do some more clacs
	dist1[i] = db;
	printf("Server: db = %d; ip = %s \n",db,temp_ip);
	found = 0;

	while(temp_ch!='\"')
	{
		temp_ch=fgetc(fp);
		if (temp_ch=='*')
			goto next1;
	}

	goto next;

next1:
	fclose(fp);
}

void send_rcv_from_helper()
{
	/*Use socket here to send the ip to helper and receive the data from there*/
	int found=0,db=0,i=0;
	char temp_ip[20],temp_ch,db_ch[5],time[50];
	FILE *fp;
	GTimeVal secs;
	fp = fopen("iw_data_help","r");
	temp_ch = fgetc(fp);
	
	if (temp_ch != '\"')
	{
		printf("Error in iwspy_data_hlp\n");
		strcpy(msgs[msg_count++],"Error in iwspy_data.\0");
		return;
	}
next2:	i=0;
	do
	{
		temp_ch = fgetc(fp);
		if (temp_ch != '\"')
			temp_ip[i++]=temp_ch;
	}
	while(temp_ch != '\"');
	
	temp_ip[i]='\0';
	for(i=0; i<no_of_ip; i++)
	{
		if (strcmp(temp_ip,track_ip[i])==0)
		{
			found++;
			break;
		}
	}
	if (found==0)
	{
		//Found a new one
		//Alert
		g_get_current_time(&secs);
		
		strcpy(track_ip[no_of_ip++],temp_ip);
		printf("Found a new ip at %s\n",g_time_val_to_iso8601(&secs));

		//strcpy(msgs[msg_count],g_time_val_to_iso8601(&secs));
		strcat(msgs[msg_count],"Found a new ip.\n");
		strcat(msgs[msg_count],track_ip[no_of_ip-1]);
		strcat(msgs[msg_count++],"\0");

	}	
	
	temp_ch = fgetc(fp);
	temp_ch = fgetc(fp);
	db=0;
	
	while(1)	
	{
		db = (db * 10) + (int) temp_ch - 48 ;	
		temp_ch = fgetc(fp);
		if (temp_ch=='d')
		{
			break;
		}
	}

	//do some more clacs
	dist2[i] = db;

	printf("Helper: db = %d; ip = %s\n",db,temp_ip);

	found = 0;

	while(temp_ch!='\"')
	{
		temp_ch=fgetc(fp);

		if (temp_ch=='*')
			goto next3;
	}

	goto next2;

next3:
	fclose(fp);
}

void helper_init()
{
	int a;
	send_rcv_from_helper();
	if (a!=0)
	{
		helper_dist = 10;//a;
		return;
	}
	else
	{
		printf("Error initialising helper.\n");
		strcpy(msgs[msg_count++],"Error in iwspy_data.\0");
		exit(1);
	}
}

void locate_and_record()
{
	/*DataBase.. Better do it in simple file. SQL might be good. Try that also.
	Pretty bad equations:
		(x*x) + (y*y) = (d1*d1)
		((b-x)*(b-x)) + (y*y) = (d2*d2) 

		y = sqrt(d1^2 - x^2)
		(b-x)^2 + (d1^2 -x^2) = d2^2 
		=>b^2 - 2bx + x^2 + d1^2 -x^2 = d2^2
		=>-2bx = d2^2 - d1^2 - b^2
		=>x = (-d2^2 + d1^2 + b^2)/2b

	  Assume that the location is towards one side of the cordinates (I). This simplifies a lot of things.
		
	  Arrange the server in a corner of the room and helper at another corner	
		^
		|
		| x
		|----*
		|d1/ | \  d2  
		| /  |y   \
		|/   |       \  
		0--------------0------>
		       b
	*/
	int x, y, b = helper_dist,i;
	char display_status[2000],ch_x[4],ch_y[4];
		
	strcpy(display_status,"Status of nodes:\n");
	for (i=0; i<no_of_ip; i++)
	{
		x = (dist1[i]*dist1[i] + b*b - dist2[i]*dist2[i])/(2*b);
		y = sqrt(dist1[i]*dist1[i] - x*x);
		x_cord[i] = x;
		y_cord[i] = y;
		itoa(x,ch_x);
		itoa(y,ch_y);
		
		strcat(display_status,track_ip[i]);
		strcat(display_status,"\t\t");
		strcat(display_status,ch_x);
		strcat(display_status,"\t");
		strcat(display_status,ch_y);
		strcat(display_status,"\n");
		gtk_label_set_text(label6, display_status);
		printf("The location of %s is %d,%d wrt the two servers\n\n",track_ip[i],x,y);
	}
		
}

void *scan_thread(void * button)
{
	int i;
	GdkRegion *region;
	button = (GtkButton *) button;
	
	while(flag)
	{
		gtk_button_set_label (button, "Stop Scanning");
		sleep(2);
		measure_strength_ip();
		send_rcv_from_helper();
		locate_and_record();

		region = gdk_drawable_get_clip_region (window1->window);
		/* redraw the cairo canvas completely by exposing it */
		gdk_window_invalidate_region (window1->window, region, TRUE);
		gdk_window_process_updates (window1->window, TRUE);
	}
	
}

void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

	static pthread_t scanner;
	if (flag==0)
		flag++;
	else
		flag--;

	if (flag)
	{
		strcpy(msgs[msg_count++],"Scanning in progress.\0");
		pthread_create(&scanner, NULL, scan_thread, button);
	}
	else
	{
		gtk_button_set_label(button, "Scan");
		strcpy(msgs[msg_count++],"Scanning stalled.\0");
	}
}


void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	char show_text[300];
	int i=0;
	strcat(track_ip[no_of_ip++], gtk_entry_get_text (entry1));
	printf("A node added: %s.\n No of nodes %d\n\n", track_ip[no_of_ip - 1], no_of_ip);
	strcpy(show_text," ");
	for (i=0; i<=no_of_ip; i++)
	{
		strcat(show_text,track_ip[i]);
		strcat(show_text,"\n");
	}
	gtk_label_set_text(label6, show_text);
		
}

gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
	cairo_t *cr;
	double mid;
	int i;
	showlabel7();
	mid = widget->allocation.x + widget->allocation.width / 2;
	
	/* get a cairo_t */
	cr = gdk_cairo_create (widget->window);

	cairo_rectangle (cr,
			event->area.x, event->area.y,

			event->area.width, event->area.height);
	cairo_clip (cr);
	
	cairo_rectangle (cr, 0, 0, widget->allocation.width, widget->allocation.height);
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_stroke (cr);
	
	cairo_move_to (cr, 5, 5);
	cairo_line_to (cr, 5, widget->allocation.height - 5);
	cairo_line_to (cr, widget->allocation.width - 5, widget->allocation.height -5);

	cairo_save (cr);
	cairo_set_line_width (cr, 0.5 *	cairo_get_line_width (cr));

	for (i=0; i<no_of_ip; i++)
	{	
		cairo_move_to (cr, 5*x_cord[i]+6, widget->allocation.height - 6 - y_cord[i]*5);
		cairo_rectangle (cr, 5*x_cord[i]+6, widget->allocation.height - 6 -y_cord[i]*5, 5, 5);
		cairo_stroke (cr);
	}
	
	cairo_restore (cr);
	cairo_destroy (cr);
  return FALSE;
}

gboolean
on_entry1_focus_event(GtkWidget       *widget,
                                        GtkDirectionType  direction,
                                        gpointer         user_data)
{
	gtk_entry_set_text(GTK_ENTRY (widget), "");
	return FALSE;
}

/*Interface*/

GtkWidget*
create_window1 (void)
{

  accel_group = gtk_accel_group_new ();
  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (window1, 800, 400);
  gtk_window_set_title (GTK_WINDOW (window1), "Main Server - WiFi Tracker");

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, TRUE, 0);

  menuitem1 = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_widget_show (menuitem1);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

  menu1 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menu1);

  imagemenuitem5 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (imagemenuitem5);
  gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem5);

  menuitem4 = gtk_menu_item_new_with_mnemonic ("_Help");
  gtk_widget_show (menuitem4);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

  menu3 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menu3);

  imagemenuitem10 = gtk_image_menu_item_new_from_stock ("gtk-about", accel_group);
  gtk_widget_show (imagemenuitem10);
  gtk_container_add (GTK_CONTAINER (menu3), imagemenuitem10);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  drawingarea1 = gtk_drawing_area_new ();
  gtk_widget_show (drawingarea1);
  gtk_box_pack_start (GTK_BOX (hbox1), drawingarea1, TRUE, TRUE, 0);

  notebook1 = gtk_notebook_new ();
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (hbox1), notebook1, FALSE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox2);

  label4 = gtk_label_new ("Usually the scanning takes place every few seconds. If you want to scan now press the button below.");
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (vbox2), label4, FALSE, TRUE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);
  gtk_label_set_line_wrap (GTK_LABEL (label4), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label4), 0.2, 0.06);

  button1 = gtk_button_new_with_mnemonic ("Scan Now");
  gtk_widget_show (button1);
  gtk_box_pack_start (GTK_BOX (vbox2), button1, FALSE, TRUE, 0);

  sw1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (sw1);
  gtk_box_pack_start (GTK_BOX (vbox2), sw1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw1), GTK_SHADOW_IN);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (sw1), viewport1);

  label7 = gtk_label_new ("");
  gtk_widget_show (label7);
  //gtk_box_pack_start (GTK_BOX (viewport1), label7, FALSE, TRUE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
  gtk_container_add (GTK_CONTAINER (viewport1), label7);
  gtk_misc_set_alignment (GTK_MISC (label7), 0.02, 0.02);
  gtk_label_set_line_wrap (label7,TRUE);

  label1 = gtk_label_new ("Common");
  gtk_widget_show (label1);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label1);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox3);

  label6 = gtk_label_new ("Status of nodes:\n");
  gtk_widget_show (label6);
  gtk_box_pack_start (GTK_BOX (vbox3), label6, TRUE, TRUE, 0);
  gtk_misc_set_alignment (GTK_MISC (label6), 0.02, 0.02);

  label2 = gtk_label_new ("Nodes");
  gtk_widget_show (label2);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);

  table1 = gtk_table_new (3, 3, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (notebook1), table1);

  comboboxentry1 = gtk_combo_box_entry_new_text ();
  gtk_widget_show (comboboxentry1);
  gtk_table_attach (GTK_TABLE (table1), comboboxentry1, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 10, 0);

  comboboxentry_entry1 = gtk_entry_new ();
  gtk_widget_show (comboboxentry_entry1);
  gtk_container_add (GTK_CONTAINER (comboboxentry1), comboboxentry_entry1);
  gtk_entry_set_text (GTK_ENTRY (comboboxentry_entry1), "wlan0");
  gtk_entry_set_invisible_char (GTK_ENTRY (comboboxentry_entry1), 9679);

  label5 = gtk_label_new ("Which device to use?");
  gtk_widget_show (label5);
  gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  label3 = gtk_label_new ("Settings");
  gtk_widget_show (label3);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label3);

  statusbar1 = gtk_statusbar_new ();
  gtk_widget_show (statusbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar1, FALSE, TRUE, 0);

  g_signal_connect ((gpointer) drawingarea1, "expose_event",
                    G_CALLBACK (on_drawingarea1_expose_event),
                    NULL);

/*  g_signal_connect ((gpointer) entry1, "focus_in_event",
                    G_CALLBACK (on_entry1_focus_event),
                    NULL);*/

  g_signal_connect ((gpointer) button1, "clicked",
                    G_CALLBACK (on_button1_clicked),
                    NULL);
/*  g_signal_connect ((gpointer) button2, "clicked",
                    G_CALLBACK (on_button2_clicked),
                    NULL);*/

  g_signal_connect ((gpointer) window1, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);

  gtk_window_add_accel_group (GTK_WINDOW (window1), accel_group);

	strcpy(msgs[msg_count++],"Waiting to scan.\0");

  return window1;
}

/*Main*/

int
main (int argc, char *argv[])
{
  GtkWidget *window1;

  gtk_init (&argc, &argv);

  window1 = create_window1 ();
  gtk_widget_show (window1);

  gtk_main ();
  return 0;
}

